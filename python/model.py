
from nonlinear import nonlinear
from statespace import state_space
from file import save_result, load_result, save_result_str
from lqdesign import lqr_design


CALCULATE_NONLINEAR = False
CALCULATE_STATE_SPACE = False
CALCULATE_LQR = True


if __name__ == "__main__":

    if CALCULATE_STATE_SPACE:
        equations, variables = nonlinear()

        eq1, eq2, eq3 = equations
        save_result_str(eq1, "nonlinear_1")
        save_result_str(eq2, "nonlinear_2")
        save_result_str(eq3, "nonlinear_3")

        A, B, C, D = state_space(equations, variables)

        save_result(A, "linear_A")
        save_result(B, "linear_B")
        save_result(C, "linear_C")
        save_result(D, "linear_D")

    if CALCULATE_LQR:

        A = load_result("linear_A")
        B = load_result("linear_B")
        C = load_result("linear_C")
        D = load_result("linear_D")

        K = lqr_design(A, B, C, D)

    # save_equations(equations, "equations.txt")