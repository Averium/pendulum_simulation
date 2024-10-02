
from nonlinear import nonlinear
from state_space import state_space


if __name__ == "__main__":
    equations, variables = nonlinear()
    
    state_space(equations, variables)
    # save_equations(equations, "equations.txt")