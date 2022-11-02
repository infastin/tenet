const std = @import("std");
const types = @import("types.zig");

const Vertex = types.Vertex;

pub fn dijkstra(allocator: std.mem.Allocator, adj_matr: [][]f64, start: usize) ![]f64 {
    const n = adj_matr.len;

    var visited: []bool = try allocator.alloc(bool, n);
    defer allocator.free(visited);
    std.mem.set(bool, visited, false);

    var marks: []f64 = try allocator.alloc(f64, n);
    std.mem.set(f64, marks, std.math.inf(f64));
    marks[start] = 0;

    var queue = std.PriorityQueue(Vertex, []f64, Vertex.order).init(allocator, marks);
    try queue.add(.{ .index = start });

    while (queue.count() != 0) {
        const cur = queue.remove();

        if (visited[cur.index]) {
            continue;
        }

        visited[cur.index] = true;

        for (adj_matr[cur.index]) |dist, index| {
            if (dist == 0) {
                continue;
            }

            const new_dist = marks[cur.index] + dist;
            if (new_dist < marks[index]) {
                marks[index] = new_dist;
                try queue.add(.{ .index = index });
            }
        }
    }

    return marks;
}

pub fn fordBellman(allocator: std.mem.Allocator, adj_matr: [][]f64, start: usize) !struct {
    dist: []f64,
    found_negative: bool,
} {
    const n = adj_matr.len;

    var dist: []f64 = try allocator.alloc(f64, n);
    std.mem.set(f64, dist, std.math.inf(f64));
    dist[start] = 0;

    var i: usize = 0;
    while (i < n - 1) : (i += 1) {
        for (adj_matr) |edges, ui| {
            for (edges) |weight, vi| {
                if (weight == 0) {
                    continue;
                }

                const new_dist = dist[ui] + weight;
                if (dist[vi] > new_dist) {
                    dist[vi] = new_dist;
                }
            }
        }
    }

    for (adj_matr) |edges, ui| {
        for (edges) |weight, vi| {
            if (dist[vi] > dist[ui] + weight) {
                return .{ .dist = dist, .found_negative = false };
            }
        }
    }

    return .{ .dist = dist, .found_negative = true };
}

pub fn floydWarshall(allocator: std.mem.Allocator, adj_matr: [][]f64) ![][]f64 {
    const n = adj_matr.len;

    var dist: [][]f64 = try allocator.alloc([]f64, n);
    for (dist) |*row, ui| {
        row.* = try allocator.alloc(f64, n);
        for (row.*) |*col, vi| {
            if (ui == vi) {
                col.* = 0;
            } else if (adj_matr[ui][vi] == 0) {
                col.* = std.math.inf(f64);
            } else {
                col.* = adj_matr[ui][vi];
            }
        }
    }

    var i: usize = 0;
    while (i < n) : (i += 1) {
        for (adj_matr) |edges, ui| {
            for (edges) |_, vi| {
                const new_dist = dist[ui][i] + dist[i][vi];
                if (dist[ui][vi] > new_dist) {
                    dist[ui][vi] = new_dist;
                }
            }
        }
    }

    return dist;
}
