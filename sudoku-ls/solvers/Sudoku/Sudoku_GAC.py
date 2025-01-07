import networkx as nx
from networkx.algorithms import bipartite
import sys

def alldifferent_filter(variables, domains):
    # Step 1: Create bipartite graph and find maximum matching
    B = nx.Graph()
    B.add_nodes_from(variables, bipartite=0)
    for var in variables:
        for val in domains[var]:
            B.add_node(val, bipartite=1)
            B.add_edge(var, val)
    matching = bipartite.maximum_matching(B, top_nodes=variables)
    
    # Step 2: Create Residual Graph
    R = nx.DiGraph()
    for var, val in matching.items():
        if var in variables:
            for value in domains[var]:
                if val == value:
                    R.add_edge(var, value)  # matched edges: variable -> value
                else:
                    R.add_edge(value, var) # unmatched edges: value -> variable
    sink = 'sink'
    R.add_node(sink)
    for node in R.nodes:
        if node in variables or node == sink:
            continue
        if node in matching.values():
            R.add_edge(node, sink)  # matched value -> sink
        else:
            R.add_edge(sink, node)  # sink -> unmatched value
            
    # Step 3: Calculate Strongly Connected Components
    SCCs = list(nx.strongly_connected_components(R))
    
    # Step 4: Remove redundant edges and simplify domains
    filtered_domains = {var: set(domains[var]) for var in variables}
    for SCC in SCCs:
        for var in SCC:
            if var not in variables:
                continue
            for pred in list(R.predecessors(var)):
                if pred not in SCC:
                    filtered_domains[var].discard(pred)
    
    return filtered_domains



def apply_GAC(variables, domains, constraints):
    old_domains = None
    fix_count = 0
    simplified_count = 0
    for var in variables:
        if len(domains[var]) == 1:
            fix_count += 1
    while old_domains != domains:
        old_domains = domains.copy()
        for constraint in constraints:
            constrained_vars = constraint
            constrained_domains = {var: domains[var] for var in constrained_vars}
            new_domains = alldifferent_filter(constrained_vars, constrained_domains)
            for var in constrained_vars:
                domains[var] = new_domains[var]
                # Check if this variable has been simplified
    for var in variables:
        if len(domains[var]) == 1:
            simplified_count += 1
    return domains, simplified_count-fix_count


def parse_sudoku(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()
    n = int(lines[0])
    size = n**2
    grid = [[int(num) for num in line.split()] for line in lines[2:]]

    # Create variables and domains
    variables = [(i, j) for i in range(size) for j in range(size)]
    domains = {(i, j): list(range(1, size+1)) if grid[i][j] == -1 else [grid[i][j]] for i, j in variables}

    # Create constraints
    constraints = []
    for i in range(size):
        constraints.append([(i, j) for j in range(size)])  # Row constraints
        constraints.append([(j, i) for j in range(size)])  # Column constraints
    for i in range(n):
        for j in range(n):
            constraints.append([(i*n+p, j*n+q) for p in range(n) for q in range(n)])  # Box constraints

    return variables, domains, constraints


def solve_sudoku(filename):
    variables, domains, constraints = parse_sudoku(filename)
    domains, simplified_count = apply_GAC(variables, domains, constraints)
    return simplified_count

if __name__ == '__main__':
    filename = sys.argv[1]
    simplified_count = solve_sudoku(filename)
    print(f'Number of variables fixed: {simplified_count}')