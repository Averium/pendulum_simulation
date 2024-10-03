from control import dlqr, c2d, ss
from file import load_params, load_result, write_tuning_params
from sympy import Matrix

from numpy import diag


def lqr_design(A, B, Q, R, dt):

    PARAMS = load_params("params")

    m1 = PARAMS["CART_MASS"]
    m2 = PARAMS["LOWER_MASS"]
    m3 = PARAMS["HIGHER_MASS"]
    l1 = PARAMS["RAIL_LENGTH"]
    l2 = PARAMS["LOWER_ROD_LENGTH"]
    l3 = PARAMS["HIGHER_ROD_LENGTH"]
    k1 = PARAMS["CART_DAMPING_RATIO"]
    k2 = PARAMS["LOWER_JOINT_DAMPING_RATIO"]
    k3 = PARAMS["HIGHER_JOINT_DAMPING_RATIO"]
    g  = PARAMS["GRAVITY"]

    values = (
        ("m1", m1),
        ("m2", m2),
        ("m3", m3),
        ("l1", l1),
        ("l2", l2),
        ("l3", l3),
        ("k1", k1),
        ("k2", k2),
        ("k3", k3),
        ("g", g),
    )

    A = A.subs(values).tolist()
    B = B.subs(values).tolist()
    C = [1, 0, 0, 0, 0, 0]
    D = [0]

    continous_system = ss(A, B, C, D)
    discrete_system = c2d(continous_system, dt, method="zoh")

    K, _, _ = dlqr(discrete_system.A, discrete_system.B, Q, R)
    return K.tolist()


if __name__ == "__main__":
    A = load_result("linear_A")
    B = load_result("linear_B")
    C = load_result("linear_C")
    D = load_result("linear_D")

    Q = [1000, 10, 100, 50, 150, 50]
    R = [0.01]

    T = 0.001

    K = lqr_design(A, B, diag(Q), diag(R), T)

    write_tuning_params(K[0])