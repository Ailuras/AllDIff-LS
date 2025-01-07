use io;

/* Read instance data */
function input() {
    local usage = "Usage: localsolver knapsack.lsp "
            + "inFileName=inputFile [solFileName=outputFile] [lsTimeLimit=timeLimit]";

    if (inFileName == nil) throw usage;

    local inFile = io.openRead(inFileName);
    nbItems = inFile.readInt();
    weights[i in 0...nbItems] = inFile.readInt();
    prices[i in 0...nbItems] = inFile.readInt();
    knapsackBound = inFile.readInt();
}

/* Declare the optimization model */
function model() {
    // Decision variables x[i]
    x[i in 0...nbItems] <- bool();

    // Weight constraint
    knapsackWeight <- sum[i in 0...nbItems](weights[i] * x[i]);
    constraint knapsackWeight <= knapsackBound;

    // Maximize value
    knapsackValue <- sum[i in 0...nbItems](prices[i] * x[i]);
    maximize knapsackValue;
}

/* Parametrize the solver */
function param() {
    if (lsTimeLimit == nil) lsTimeLimit = 20; 
}

/* Write the solution in a file */
function output() {
    if (solFileName == nil) return;
    local solFile = io.openWrite(solFileName);
    solFile.println(knapsackValue.value);
    for [i in 0...nbItems : x[i].value == 1]
        solFile.print(i + " ");
    solFile.println();
}
