from sympy import symbols, sin, cos, simplify, diff, Eq, expand, factor
from sympy.physics.vector import ReferenceFrame
from sympy.physics.mechanics import dynamicsymbols


REPLACE = (
    ("Derivative(q1(t), t)", "dq1"),
    ("Derivative(q2(t), t)", "dq2"),
    ("Derivative(q3(t), t)", "dq3"),
    ("Derivative(q1(t), (t, 2))", "ddq1"),
    ("Derivative(q2(t), (t, 2))", "ddq2"),
    ("Derivative(q3(t), (t, 2))", "ddq3"),
    ("q1(t)", "q1"),
    ("q2(t)", "q2"),
    ("q3(t)", "q3"),
)

REPLACE_EXPR = (
    ("q2 - q3", "q2-q3"),
    ("Eq(", ""),
    (" + ", "\n+ "),
    (" - ", "\n- "),
    (", ", "\n=\n"),
)

def print_expr(symbol):
    text = str(symbol)
    for args in REPLACE + REPLACE_EXPR:
        text = text.replace(*args)

    for i, char in enumerate(text):
        if char == "\n":
            if text[i+1] not in ("\n", "=", "-", "+"):
                text = text[:i] + "@" + text[i+1:]

    text = text.replace("@", "\n+ ")

    if text[0] == "-":
        text = "- " + text[1:-1] + "\n\n"
    else:
        text = "+ " + text[:-1] + "\n\n"

    return text


def print_eq(symbol):
    text = str(symbol)
    for args in REPLACE:
        text = text.replace(*args)
    return text + "\n\n"


def diffv(vector, symbol, frame):
    result = \
    + diff(vector.dot(frame.x), symbol) * frame.x \
    + diff(vector.dot(frame.y), symbol) * frame.y \
    + diff(vector.dot(frame.z), symbol) * frame.z \
    
    return result

TEST = False

if not TEST:

    m1, m2, m3 = symbols("m1, m2, m3")  # cart mass, lower pendulum mass, higher pendulum mass #
    l1, l2, l3 = symbols("l1, l2, l3,") # rail length (not used), lower rod length, higher rod length #
    k1, k2, k3 = symbols("k1, k2, k3")  # friction coefficients for rail, lower joint, upper joint #
    F, g = symbols("F, g")              # force for controlling the pendulum and gravity acceleration #

    q1, q2, q3 = dynamicsymbols("q1, q2, q3") # degrees of freedom: q1 = x coord of cart, q2 = lower angle, q3 = higher angle
    dq1, dq2, dq3 = q1.diff(), q2.diff(), q3.diff()
    ddq1, ddq2, ddq3 = dq1.diff(), dq2.diff(), dq3.diff()

    # REFERENCE FRAME #

    R = ReferenceFrame("R")
    i_ = R.x
    j_ = R.y
    k_ = R.z

    # POSITION VECTORS #

    r12_ = -sin(q2) * l2 * i_ + cos(q2) * l2 * j_
    r23_ = -sin(q3) * l3 * i_ + cos(q3) * l3 * j_

    # VELOCITY VECTORS #

    v1_ = dq1 * i_
    v2_ = v1_ + (dq2 * k_).cross(r12_)
    v3_ = v2_ + (dq3 * k_).cross(r23_)

    w1_ = 0 * k_
    w2_ = dq2 * k_
    w3_ = dq3 * k_

    # VELOCITY VECTOR MAGNITUDES #

    v1_sqr = simplify(v1_.dot(i_)**2 + v1_.dot(j_)**2)
    v2_sqr = simplify(v2_.dot(i_)**2 + v2_.dot(j_)**2)
    v3_sqr = simplify(v3_.dot(i_)**2 + v3_.dot(j_)**2)

    # KINETIC ENERGY #

    E1 = m1 * v1_sqr / 2
    E2 = m2 * v2_sqr / 2
    E3 = m3 * v3_sqr / 2
    E = E1 + E2 + E3

    # DERIVATIVES #

    DE_Ddq1 = simplify(diff(E, dq1).doit())
    DE_Ddq2 = simplify(diff(E, dq2).doit())
    DE_Ddq3 = simplify(diff(E, dq3).doit())

    dDE_Ddq1_dt = simplify(diff(E, dq1).doit().diff("t"))
    dDE_Ddq2_dt = simplify(diff(E, dq2).doit().diff("t"))
    dDE_Ddq3_dt = simplify(diff(E, dq3).doit().diff("t"))

    DE_Dq1 = simplify(diff(E, q1).doit())
    DE_Dq2 = simplify(diff(E, q2).doit())
    DE_Dq3 = simplify(diff(E, q3).doit())

    # FORCE AND TORQUE VECTORS #

    F_ = F * i_

    M1_ = 0 * k_
    M2_ = -k2 * w2_
    M3_ = -k3 * w3_

    F1_ = F_ - k1 * v1_ - m1 * g * j_
    F2_ = - m2 * g * j_
    F3_ = - m3 * g * j_

    # Q FORCE CALCULATION #

    B11_ = diffv(v1_, dq1, R)
    B21_ = diffv(v2_, dq1, R)
    B31_ = diffv(v3_, dq1, R)

    B12_ = diffv(v1_, dq2, R)
    B22_ = diffv(v2_, dq2, R)
    B32_ = diffv(v3_, dq2, R)

    B13_ = diffv(v1_, dq3, R)
    B23_ = diffv(v2_, dq3, R)
    B33_ = diffv(v3_, dq3, R)

    b11_ = diffv(w1_, dq1, R)
    b21_ = diffv(w2_, dq1, R)
    b31_ = diffv(w3_, dq1, R)

    b12_ = diffv(w1_, dq2, R)
    b22_ = diffv(w2_, dq2, R)
    b32_ = diffv(w3_, dq2, R)
    
    b13_ = diffv(w1_, dq3, R)
    b23_ = diffv(w2_, dq3, R)
    b33_ = diffv(w3_, dq3, R)

    Q1 = F1_.dot(B11_) + F2_.dot(B21_) + F3_.dot(B31_) + M1_.dot(b11_) + M2_.dot(b21_) + M3_.dot(b31_)
    Q2 = F1_.dot(B12_) + F2_.dot(B22_) + F3_.dot(B32_) + M1_.dot(b12_) + M2_.dot(b22_) + M3_.dot(b32_)
    Q3 = F1_.dot(B13_) + F2_.dot(B23_) + F3_.dot(B33_) + M1_.dot(b13_) + M2_.dot(b23_) + M3_.dot(b33_)

    # EQUATIONS #

    eq1 = Eq(dDE_Ddq1_dt - DE_Dq1, Q1)
    eq2 = Eq(dDE_Ddq2_dt - DE_Dq2, Q2)
    eq3 = Eq(dDE_Ddq3_dt - DE_Dq3, Q3)

    eq1 = expand(eq1)
    eq2 = expand(eq2)
    eq3 = expand(eq3)

else:

    eq1 = "eq1"
    eq2 = "eq2"
    eq3 = "eq3\n"

with open("equations.txt", "w+") as FILE:
    e1 = print_expr(eq1)
    e2 = print_expr(eq2)
    e3 = print_expr(eq3)

    FILE.writelines((e1, e2, e3))