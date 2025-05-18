async function init() {
  const { instance } = await WebAssembly.instantiateStreaming(
    fetch("./add.wasm")
  );
  console.log(instance.exports.add(4, 1));

  return instance.exports;
}

init().then((result) => {
    console.log(result);
        }
           );

const adjacencyMatrix = [
  [0, 1, 0, 0],
  [1, 0, 1, 1],
  [0, 1, 0, 0],
  [0, 1, 0, 0]
];

// Generate nodes from adjacency matrix
var nodes = adjacencyMatrix.map((_, i) => ({ id: i }));

// Generate links from adjacency matrix
var links = [];
for (let i = 0; i < adjacencyMatrix.length; i++) {
  for (let j = 0; j < adjacencyMatrix[i].length; j++) {
    if (adjacencyMatrix[i][j]) {
      links.push({ source: i, target: j });
    }
  }
}
