let wasmExports = null; // To store the WASM exports once loaded

// Create a flat array for the matrix
const MATRIX_SIZE = 64;
let adjMatrix = new Int32Array(MATRIX_SIZE * MATRIX_SIZE);

let active_node_count = -1;

// D3 variables
let svg, link, node;
let links = [], nodes = [];

// Color scale for nodes
const colorScale = d3.scaleOrdinal()
    .domain([1, 2, 3, 4])
    .range(['#69b3a2', '#ff7f0e', '#2ca02c', '#d62728']);

function cstrlen(mem, ptr) {
    console.log("cstrlen", mem, ptr);
    let len = 0;
    while (mem[ptr] != 0 && len < 128) {
        len++;
        ptr++;
    }
    console.log("len", len);
    return len;
}

function cstr_by_ptr(mem_buffer, ptr) {
    const mem = new Uint8Array(mem_buffer);
    const len = cstrlen(mem, ptr);
    const bytes = new Uint8Array(mem_buffer, ptr, len);
    return new TextDecoder().decode(bytes);
}
function delete_node(node) {
    // get the adjacency matrix
    const memory = new Int32Array(wasmExports.memory.buffer);
    const adjMatrix = memory.slice(0, MATRIX_SIZE * MATRIX_SIZE);

    // delete node by removing all its edges
    for (let i = 0; i < MATRIX_SIZE; i++) {
        adjMatrix[node * MATRIX_SIZE + i] = 0;
        adjMatrix[i * MATRIX_SIZE + node] = 0;
    }
}
function updateGraph() {
    // Convert adjacency matrix to graph data
    links = [];
    nodes = [];
    
    const memory = new Int32Array(wasmExports.memory.buffer);
    const offset = wasmExports.get_node_colors();
    // Create nodes
    for (let i = 0; i < active_node_count; i++) {
        // Get color from WebAssembly memory
        const colorIndex = memory[offset /4+ i]; // Color array starts after the adjacency matrix
        nodes.push({ 
            id: i,
            color: colorIndex > 0 ? colorScale(colorIndex) : '#69b3a2' // Default color if no color assigned
        });
    }
    
    // Create links from adjacency matrix
    for (let i = 0; i < active_node_count; i++) {
        for (let j = 0; j < active_node_count; j++) {
            for (let k = 0; k < adjMatrix[i*MATRIX_SIZE+j]; k++) {
                links.push({ source: i, target: j });
            }
        }
    }

    // Update the links
    link = svg
        .selectAll("line")
        .data(links)
        .join("line")
        .style("stroke", "#aaa");

    // Update the nodes
    node = svg
        .selectAll("circle")
        .data(nodes)
        .join("circle")
        .attr("r", 20)
        .style("fill", d => d.color);

    // Update the simulation
    const simulation = d3.forceSimulation(nodes)
        .force("link", d3.forceLink()
            .id(d => d.id)
            .links(links))
        .force("charge", d3.forceManyBody().strength(-400))
        .force("center", d3.forceCenter(400, 400))
        .on("tick", ticked);

    function ticked() {
        link
            .attr("x1", d => d.source.x)
            .attr("y1", d => d.source.y)
            .attr("x2", d => d.target.x)
            .attr("y2", d => d.target.y);

        node
            .attr("cx", d => d.x)
            .attr("cy", d => d.y);
    }

    // Add drag behavior
    node.call(d3.drag()
        .on("start", dragstarted)
        .on("drag", dragged)
        .on("end", dragended));

    function dragstarted(event) {
        if (!event.active) simulation.alphaTarget(0.3).restart();
        event.subject.fx = event.subject.x;
        event.subject.fy = event.subject.y;
    }

    function dragged(event) {
        event.subject.fx = event.x;
        event.subject.fy = event.y;
    }

    function dragended(event) {
        if (!event.active) simulation.alphaTarget(0);
        event.subject.fx = null;
        event.subject.fy = null;
    }
}

function decodeStr(ptr, len) {
    const memory = new Uint8Array(wasmExports.memory.buffer);
    let str = '';
    for (let i = 0; i < len; i++) {
        const c = memory[ptr + i];
        if (c == 0) {
            break;
        }
        str += String.fromCharCode(c);
    }
    return str;
}

document.addEventListener('DOMContentLoaded', () => {
    // Initialize D3 visualization
    const margin = { top: 10, right: 30, bottom: 30, left: 40 },
        width = 800 - margin.left - margin.right,
        height = 800 - margin.top - margin.bottom;

    svg = d3.select("#my_dataviz")
        .append("svg")
        .attr("width", width + margin.left + margin.right)
        .attr("height", height + margin.top + margin.bottom)
        .append("g")
        .attr("transform", `translate(${margin.left}, ${margin.top})`);

    const fileInput = document.getElementById('load_wasm');

    fileInput.addEventListener('change', async (event) => {
        const file = event.target.files[0];
        if (!file) {
            console.log("No file selected.");
            return;
        }

        if (file.name.split('.').pop().toLowerCase() !== 'wasm') {
            alert("Please upload a .wasm file.");
            fileInput.value = ''; // Clear the input
            return;
        }

        console.log(`Selected file: ${file.name}`);

        try {
            const importObject = {
                env: {
                    jsLog: (ptr) => {
                        const msg = decodeStr(ptr, 128);
                        console.log("msg", msg);
                        document.querySelector("#log").textContent += ("\n" + msg);
                    },
                    updateGraph: updateGraph,
                    merge_nodes: merge_nodes,
                    rand: () => {
                        return Math.floor(Math.random() * 16);
                    }
                }
            };

            // Instantiate the WebAssembly module from the file stream
            const { instance } = await WebAssembly.instantiateStreaming(new Response(file), importObject);

            wasmExports = instance.exports;
            console.log("WASM module loaded successfully!");

            console.log("WASM exports:", wasmExports);

            if (wasmExports.init) {
                if (!document.getElementById("graph_from_wasm").checked) {
                    console.log("Graph from module is off, skipping init");
                    return;
                }

                active_node_count = wasmExports.get_active_node_count();

                const offset = wasmExports.get_adjMatrix();

                // Copy the matrix to WebAssembly memory
                const memory = new Int32Array(wasmExports.memory.buffer);
                memory.set(adjMatrix, offset);
                wasmExports.init(offset); // Pass the offset in memory
                // Copy the modified matrix back
                adjMatrix.set(memory.slice(offset / 4, offset / 4 + MATRIX_SIZE * MATRIX_SIZE));
                // Update the graph visualization
                updateGraph();
            } else {
                console.warn("The loaded WASM module does not have a 'init' function.");
            }

        } catch (error) {
            console.error("Error loading WASM file:", error);
            alert("Failed to load WASM file. Check console for details.");
        }
    });

    // The original init function (modified to be called on file selection)
    // async function init() {
    //     // This function is now superseded by the file input's change event listener
    //     // where the WASM is loaded from the selected file.
    //     // If you still need a default WASM to load on page load, you can keep a modified version here.
    // }

    // You can optionally call the init function if you want to load a default WASM on page load
    // For example, if you have a default `add.wasm` in the same directory:
    /*
    async function loadDefaultWASM() {
        try {
            const { instance } = await WebAssembly.instantiateStreaming(
                fetch("./add.wasm")
            );
            wasmExports = instance.exports;
            console.log("Default WASM loaded successfully!");
            if (wasmExports.add) {
                console.log(`Result of default add(4, 1): ${wasmExports.add(4, 1)}`);
            }
        } catch (error) {
            console.warn("Could not load default WASM (add.wasm). Please select a file.", error);
        }
    }
    loadDefaultWASM();
    */
});

function merge_nodes(node1, node2) {
    // get the adjacency matrix
    const memory = new Int32Array(wasmExports.memory.buffer);
    const adjMatrix = memory.slice(0, MATRIX_SIZE * MATRIX_SIZE);

    // merge node2 into node1 by combining their edges
    for (let i = 0; i < MATRIX_SIZE; i++) {
        // If either node1 or node2 has an edge to node i, node1 will have that edge
        if (adjMatrix[node2 * MATRIX_SIZE + i] > 0 || adjMatrix[node1 * MATRIX_SIZE + i] > 0) {
            adjMatrix[node1 * MATRIX_SIZE + i] = 1;
            adjMatrix[i * MATRIX_SIZE + node1] = 1; // Make it symmetric
        }
    }

    // Remove self-loops
    adjMatrix[node1 * MATRIX_SIZE + node1] = 0;

    // Copy the updated matrix back to WebAssembly memory
    memory.set(adjMatrix);

    // Update the graph visualization
    updateGraph();
}

function step() {
    if (!wasmExports.step) {
        console.warn("The loaded WASM module does not have a 'step' function.");
        return;
    }

    const memory = new Int32Array(wasmExports.memory.buffer);
    const offset = wasmExports.get_adjMatrix();
    memory.set(adjMatrix, offset);
    wasmExports.step(offset); // Pass the offset in memory
    adjMatrix.set(memory.slice(offset / 4, offset / 4 + MATRIX_SIZE * MATRIX_SIZE));

    active_node_count = wasmExports.get_active_node_count();

    updateGraph();
}