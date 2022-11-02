const std = @import("std");

pub const Vertex = struct {
    const Self = @This();

    index: usize,

    pub fn order(context: []f64, lhs: Self, rhs: Self) std.math.Order {
        return std.math.order(context[lhs.index], context[rhs.index]);
    }
};

pub const Edge = struct {
    const Self = @This();

    u: usize,
    v: usize,

    pub fn order(context: [][]f64, lhs: Self, rhs: Self) std.math.Order {
        return std.math.order(context[lhs.u][lhs.v], context[rhs.u][rhs.v]);
    }

    pub fn lessThan(context: [][]f64, lhs: Self, rhs: Self) bool {
        return context[lhs.u][lhs.v] < context[rhs.u][rhs.v];
    }
};
