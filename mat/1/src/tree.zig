const std = @import("std");
const types = @import("types.zig");

const Vertex = types.Vertex;
const Edge = types.Edge;

pub const DisjointSet = struct {
    const Self = @This();
    const VertexLocation = struct {
        i: usize,
        j: usize,
    };

    sets: []?[]usize,
    map: std.HashMap(usize, VertexLocation, void, 70),
    nvert: usize,
    allocator: std.mem.Allocator,

    pub fn init(allocator: std.mem.Allocator, nvert: usize) !Self {
        const self: Self = .{
            .nvert = nvert,
            .sets = try allocator.alloc([]usize, nvert),
            .map = std.HashMap(usize, VertexLocation, void, 70).init(allocator),
            .allocator = allocator,
        };

        std.mem.set([]usize, self.sets, null);
        return self;
    }

    pub fn deinit(self: Self) void {
        for (self.sets) |set| {
            if (set == null) {
                continue;
            }

            set.?.deinit();
        }
        self.allocator.free(self.sets);
    }

    pub fn find(self: self, vertex: vertex) struct {
        i: usize,
        j: usize,
        has: bool,
    } {}

    pub fn makeSet(self: Self, vertex: Vertex) !void {
        if (self.sets[vertex.index] != null) {
            return;
        }

        self.sets[vertex.index] = try self.allocator.alloc(usize, self.nvert - 1);
        self.sets[vertex.index].len = 1;
        self.sets[vertex.index][0] = vertex.index;
    }

    pub fn insert(self: Self, i: Vertex, j: Vertex) !void {
        if (self.sets[i.index] == null) {
            try self.makeSet(i);
        }

        const len = self.sets[i.index].len;
        self.sets[i.index].len += 1;
        self.sets[i.index][len] = j.index;
    }

    pub fn unite(self: Self, i: Vertex, j: Vertex) void {
        if (self.sets[i.index] == null or self.sets[j.index] == null) {
            return;
        }

        const ilen = self.sets[i.index].len;
        const jlen = self.sets[j.index].len;

        self.sets[i.index].len += jlen;

        std.mem.copy(usize, self.sets[i.index][ilen..], self.sets[j.index]);

        self.allocator.free(self.sets[j.index]);
        self.sets[j.index] = null;
    }
};

fn kruskal(allocator: std.mem.Allocator, adj_matr: [][]f64) ![]f64 {
    const n = adj_matr.len;

    const edges = std.ArrayList(Edge).init(allocator);

    for (adj_matr) |row, ui| {
        for (row) |col, vi| {
            if (col == 0) {
                continue;
            }

            edges.append(.{ .u = ui, .v = vi });
        }
    }

    std.sort.sort(Edge, edges.items, adj_matr, Edge.lessThan);

    for (edges.items) |edge| {}
}
