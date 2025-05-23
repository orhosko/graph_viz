const std = @import("std");

const allocator = std.heap.wasm_allocator;

/// Add a to b, wrapping on overflow.
export fn add(a: i32, b: i32) i32 {
    return a +% b;
}

// export fn adj_matrix() [4]u8 {
//     const size = 4;
//     const matrix: [size][size]u8 = [_][size]u8{
//         [_]u8{ 0, 1, 2, 3 },
//         [_]u8{ 4, 5, 6, 7 },
//         [_]u8{ 8, 9, 10, 11 },
//         [_]u8{ 12, 13, 14, 15 },
//     };
//     return matrix[0][0..];
// }

/// Subtract b from a, wrapping on overflow.
pub export fn sub(a: i32, b: i32) i32 {
    return a -% b;
}

/// Allocate `len` bytes in WASM memory. Returns
/// many item pointer on success, null on error.
pub export fn alloc(len: usize) ?[*]u8 {
    return if (allocator.alloc(u8, len)) |slice|
        slice.ptr
    else |_|
        null;
}

/// Free `len` bytes in WASM memory pointed to by `ptr`.
pub export fn free(ptr: ?[*]u8, len: usize) void {
    if (ptr) |_ptr| allocator.free(_ptr[0..len]);
}

/// Called from JS but turns around and calls back to JS
/// in order to log to the JS console.
pub export fn zlog(ptr: [*]const u8, len: usize) void {
    jsLog(ptr, len);
}

/// Log to the JS console.
extern "env" fn jsLog(ptr: [*]const u8, len: usize) void;

const ArrayList = std.ArrayList;
const HashMap = std.HashMap;
const json = std.json;

// Define the structure for a node in the JSON output
const Node = struct {
    id: u32,
    name: []const u8,

    const Self = @This();

    pub fn encode(value: Self, writer: anytype, _: json.EncodeOptions) !void {
        try writer.beginObject();
        try writer.objectField("id");
        try writer.writeNumber(value.id);
        try writer.objectField("name");
        try writer.writeString(value.name);
        try writer.endObject();
    }
};

// Define the structure for a link (edge) in the JSON output
const Link = struct {
    source: u32,
    target: u32,

    const Self = @This();

    pub fn encode(value: Self, writer: anytype, _: json.EncodeOptions) !void {
        try writer.beginObject();
        try writer.objectField("source");
        try writer.writeNumber(value.source);
        try writer.objectField("target");
        try writer.writeNumber(value.target);
        try writer.endObject();
    }
};

// Define the top-level graph structure for JSON output
const GraphData = struct {
    nodes: ArrayList(Node),
    links: ArrayList(Link),

    const Self = @This();

    pub fn encode(value: Self, writer: anytype, options: json.EncodeOptions) !void {
        try writer.beginObject();
        try writer.objectField("nodes");
        try writer.writeArray(value.nodes.items, options); // Encode nodes array
        try writer.objectField("links");
        try writer.writeArray(value.links.items, options); // Encode links array
        try writer.endObject();
    }
};

/// Parses an adjacency matrix from a string and converts it to GraphData.
/// Assumes the matrix is square and contains '0' or '1' characters separated by spaces or newlines.
/// The number of nodes is inferred from the matrix dimensions.
pub fn adjacencyMatrixToGraphData(
    matrix_str: []const u8,
) !GraphData {
    var nodes = ArrayList(Node).init(allocator);
    var links = ArrayList(Link).init(allocator);

    var lines_it = std.mem.tokenizeAny(u8, matrix_str, "\n");
    var matrix = ArrayList(ArrayList(u32)).init(allocator);

    // Parse the matrix string into a 2D array of integers
    while (lines_it.next()) |line| {
        const trimmed_line = std.mem.trim(u8, line, " \r\t");
        if (trimmed_line.len == 0) continue; // Skip empty lines

        var row = ArrayList(u32).init(allocator);
        var chars_it = std.mem.tokenizeAny(u8, trimmed_line, " ");
        while (chars_it.next()) |char_val| {
            if (char_val.len != 1) {
                return error.InvalidMatrixFormat;
            }
            const val = char_val[0];
            if (val == '0') {
                try row.append(0);
            } else if (val == '1') {
                try row.append(1);
            } else {
                return error.InvalidMatrixValue;
            }
        }
        if (row.items.len > 0) {
            try matrix.append(row);
        } else {
            row.deinit(); // Deinit empty row if not appended
        }
    }

    if (matrix.items.len == 0) {
        return error.EmptyMatrix;
    }

    const num_nodes = matrix.items.len;

    // Validate if it's a square matrix and create nodes
    for (0..num_nodes) |i| {
        if (matrix.items[i].items.len != num_nodes) {
            return error.NonSquareMatrix;
        }
        // Generate node names A, B, C...
        const name_char: u8 = 'A' + @as(u8, @intCast(i));
        var name_buf: [2]u8 = undefined; // For single char names like "A"
        name_buf[0] = name_char;
        name_buf[1] = 0; // Null terminator
        const name_slice = try allocator.dupe(u8, name_buf[0..1]); // Allocate a slice for the name
        try nodes.append(.{ .id = i + 1, .name = name_slice });
    }

    // Generate links from the adjacency matrix
    for (0..num_nodes) |r_idx| {
        for (0..num_nodes) |c_idx| {
            if (matrix.items[r_idx].items[c_idx] == 1) {
                // Node IDs are 1-based in the desired output
                try links.append(.{ .source = r_idx + 1, .target = c_idx + 1 });
            }
        }
    }

    // Clean up temporary matrix data
    for (matrix.items) |row| {
        row.deinit();
    }
    matrix.deinit();

    return GraphData{ .nodes = nodes, .links = links };
}

pub export fn testa() void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    // Example adjacency matrix (10x10 to match your example output)
    const adjacency_matrix =
        \\0 1 0 0 1 1 0 0 0 0
        \\0 0 1 0 0 0 1 0 0 0
        \\0 0 0 1 0 0 0 1 0 0
        \\0 0 0 0 1 0 0 0 1 0
        \\0 0 0 0 0 0 0 0 0 1
        \\0 0 0 0 0 0 0 0 0 0
        \\0 0 0 0 0 0 0 0 0 0
        \\0 0 0 0 0 0 0 0 0 0
        \\0 0 0 0 0 0 0 0 0 0
        \\0 0 0 0 0 0 0 0 0 0
    ;

    var graph_data = try adjacencyMatrixToGraphData(adjacency_matrix);
    defer {
        // Deallocate dynamically allocated node names
        for (graph_data.nodes.items) |node| {
            allocator.free(node.name);
        }
        graph_data.nodes.deinit();
        graph_data.links.deinit();
    }

    var writer = std.io.bufferedWriter(std.io.getStdOut().writer());
    defer writer.flush() catch {}; // Ensure all buffered output is written

    const encode_options = json.EncodeOptions{ .whitespace = .indent_4 }; // Pretty print

    try json.stringify(writer.writer(), graph_data, encode_options);
    try writer.writer().writeAll("\n"); // Add a newline at the end for clean output
}
