const std = @import("std");
const path = @import("path.zig");
const tree = @import("tree.zig");

const stderr = std.io.getStdErr().writer();
const stdout = std.io.getStdOut().writer();

fn readNumber(comptime T: type, comptime is_float: bool, file: std.fs.File) !T {
    var buffer: [512]u8 = undefined;
    var i: usize = 0;

    while (file.reader().readByte()) |c| {
        if (i == 512) {
            return error.StreamTooLong;
        }

        if (c == ' ' or c == '\n' or c == '\t' or c == '\r') {
            if (i != 0) {
                break;
            }
            continue;
        }

        buffer[i] = c;
        i += 1;
    } else |err| {
        if (err != error.EndOfStream) {
            return err;
        }
    }

    if (i == 0) {
        return error.EndOfStream;
    }

    if (is_float) {
        return try std.fmt.parseFloat(T, buffer[0..i]);
    }

    return try std.fmt.parseInt(T, buffer[0..i], 10);
}

pub fn main() anyerror!void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();

    const args = try std.process.argsAlloc(allocator);
    defer std.process.argsFree(allocator, args);

    if (args.len != 3) {
        try stderr.print("Expected 2 arguments, got {d}\n", .{args.len});
        std.process.exit(1);
    }

    var file: std.fs.File = std.fs.cwd().openFile(args[1], .{}) catch |err| {
        try stderr.print("Couldn't open a file {s}: {s}\n", .{ args[1], @errorName(err) });
        std.process.exit(1);
    };
    defer file.close();

    var arena = std.heap.ArenaAllocator.init(allocator);
    defer arena.deinit();
    const arena_allocator = arena.allocator();

    const n: usize = try readNumber(usize, false, file);

    const smatr: [][]f64 = try arena_allocator.alloc([]f64, n);
    for (smatr) |*row| {
        row.* = try arena_allocator.alloc(f64, n);
        for (row.*) |*col| {
            col.* = try readNumber(f64, true, file);
        }
    }

    const start = try std.fmt.parseInt(usize, args[2], 10);
    const alg1 = try path.dijkstra(arena_allocator, smatr, start);

    try stdout.print("Dijkstra:\n", .{});
    for (alg1) |dist| {
        try stdout.print("{d} ", .{dist});
    }
    try stdout.print("\n", .{});

    const alg2 = try path.fordBellman(arena_allocator, smatr, start);

    try stdout.print("Ford-Bellman:\n", .{});
    if (alg2.found_negative) {
        try stdout.print("Found negative cycle", .{});
    } else {
        for (alg2.dist) |dist| {
            try stdout.print("{d} ", .{dist});
        }
    }
    try stdout.print("\n", .{});

    const alg3 = try path.floydWarshall(arena_allocator, smatr);

    try stdout.print("Floyd-Warshall:\n", .{});
    for (alg3) |row| {
        for (row) |col| {
            try stdout.print("{d} ", .{col});
        }
        try stdout.print("\n", .{});
    }
}
