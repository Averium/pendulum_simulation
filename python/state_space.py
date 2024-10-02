
from sympy import symbols, linear_eq_to_matrix, Matrix, simplify, diff
from tools import print_eq


def state_space(equations, variables):
    q1, q2, q3, dq1, dq2, dq3, ddq1, ddq2, ddq3 = variables

    M, b = linear_eq_to_matrix(equations, (ddq1, ddq2, ddq3))
    Q = M.inv() * b

    print("MODEL: highest derivatives found")

    ddq1_nonlin = Q[0]
    ddq2_nonlin = Q[1]
    ddq3_nonlin = Q[2]

    x1, x2, x3, x4, x5, x6, F = symbols("x1, x2, x3, x4, x5, x6, F")

    x1 = q1
    x2 = dq1
    x3 = q2
    x4 = dq2
    x5 = q3
    x6 = dq3

    dx1 = x2
    dx2 = ddq1_nonlin
    dx3 = x4
    dx4 = ddq2_nonlin
    dx5 = x6
    dx6 = ddq3_nonlin

    state = [x1, x2, x3, x4, x5, x6]
    functions = [dx1, dx2, dx3, dx4, dx5, dx6]

    equillibrium = [(x1, 0), (x2, 0), (x3, 0), (x4, 0), (x5, 0), (x6, 0)]

    print("MODEL: state variables and nonlinear functions calculated")

    A = Matrix([[0 for col in range(6)] for row in range(6)])

    for row, function in enumerate(functions):
        for col, variable in enumerate(state):
            A[row, col] = simplify(diff(function, variable).doit().subs(equillibrium))
            print_eq(f"A[{row}, {col}] = {A[row, col]}")
        print()

    B = Matrix([[F], [0], [0], [0], [0], [0]])
    C = Matrix([[0, 0, 0, 0, 0, 0]])
    D = Matrix([[0]])

    print("MODEL: linearized model created")