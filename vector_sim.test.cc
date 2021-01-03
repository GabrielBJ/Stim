#include "gtest/gtest.h"
#include "vector_sim.h"

static float complex_distance(std::complex<float> a, std::complex<float> b) {
    auto d = a - b;
    return sqrtf(d.real()*d.real() + d.imag()*d.imag());
}

#define ASSERT_NEAR_C(a, b) ASSERT_LE(complex_distance(a, b), 1e-4)

TEST(vector_sim, qubit_order) {
    VectorSim sim(2);
    sim.apply("H", 0);
    sim.apply("CNOT", 0, 1);
    ASSERT_NEAR_C(sim.state[0], sqrtf(0.5));
    ASSERT_NEAR_C(sim.state[1], 0);
    ASSERT_NEAR_C(sim.state[2], 0);
    ASSERT_NEAR_C(sim.state[3], sqrtf(0.5));
}

TEST(vector_sim, h_squared) {
    VectorSim sim(1);
    sim.apply("H", 0);
    sim.apply("H", 0);
    ASSERT_NEAR_C(sim.state[0], 1);
    ASSERT_NEAR_C(sim.state[1], 0);
}

TEST(vector_sim, sqrt_x_squared) {
    VectorSim sim(1);
    sim.apply("SQRT_X_DAG", 0);
    sim.apply("SQRT_X_DAG", 0);
    ASSERT_NEAR_C(sim.state[0], 0);
    ASSERT_NEAR_C(sim.state[1], 1);
}

TEST(vector_sim, state_channel_duality_cnot) {
    VectorSim sim(4);
    sim.apply("H", 0);
    sim.apply("H", 1);
    sim.apply("CNOT", 0, 2);
    sim.apply("CNOT", 1, 3);
    sim.apply("CNOT", 2, 3);
    for (size_t row = 0; row < 4; row++) {
        for (size_t col = 0; col < 4; col++) {
            ASSERT_NEAR_C(sim.state[row*4 + col], GATE_UNITARIES.at("CNOT")[row][col]*0.5f);
        }
    }
}

TEST(vector_sim, state_channel_duality_y) {
    VectorSim sim(2);
    sim.apply("H", 0);
    sim.apply("CNOT", 0, 1);
    sim.apply("Y", 1);
    for (size_t row = 0; row < 2; row++) {
        for (size_t col = 0; col < 2; col++) {
            ASSERT_NEAR_C(sim.state[row*2 + col], GATE_UNITARIES.at("Y")[row][col]*sqrtf(0.5f));
        }
    }
}

TEST(vector_sim, apply_pauli) {
    VectorSim sim(2);

    sim.apply(PauliString::from_str("+II"), 0);
    ASSERT_NEAR_C(sim.state[0], 1);
    ASSERT_NEAR_C(sim.state[1], 0);
    ASSERT_NEAR_C(sim.state[2], 0);
    ASSERT_NEAR_C(sim.state[3], 0);

    sim.apply(PauliString::from_str("-II"), 0);
    ASSERT_NEAR_C(sim.state[0], -1);
    ASSERT_NEAR_C(sim.state[1], 0);
    ASSERT_NEAR_C(sim.state[2], 0);
    ASSERT_NEAR_C(sim.state[3], 0);

    sim.apply(PauliString::from_str("+XI"), 0);
    ASSERT_NEAR_C(sim.state[0], 0);
    ASSERT_NEAR_C(sim.state[1], -1);
    ASSERT_NEAR_C(sim.state[2], 0);
    ASSERT_NEAR_C(sim.state[3], 0);

    sim.apply(PauliString::from_str("+IZ"), 0);
    ASSERT_NEAR_C(sim.state[0], 0);
    ASSERT_NEAR_C(sim.state[1], -1);
    ASSERT_NEAR_C(sim.state[2], 0);
    ASSERT_NEAR_C(sim.state[3], 0);

    sim.apply(PauliString::from_str("+ZI"), 0);
    ASSERT_NEAR_C(sim.state[0], 0);
    ASSERT_NEAR_C(sim.state[1], 1);
    ASSERT_NEAR_C(sim.state[2], 0);
    ASSERT_NEAR_C(sim.state[3], 0);

    sim.apply(PauliString::from_str("+IY"), 0);
    ASSERT_NEAR_C(sim.state[0], 0);
    ASSERT_NEAR_C(sim.state[1], 0);
    ASSERT_NEAR_C(sim.state[2], 0);
    ASSERT_NEAR_C(sim.state[3], std::complex<float>(0, 1));

    sim.apply(PauliString::from_str("+XX"), 0);
    ASSERT_NEAR_C(sim.state[0], std::complex<float>(0, 1));
    ASSERT_NEAR_C(sim.state[1], 0);
    ASSERT_NEAR_C(sim.state[2], 0);
    ASSERT_NEAR_C(sim.state[3], 0);

    sim.apply(PauliString::from_str("+X"), 1);
    ASSERT_NEAR_C(sim.state[0], 0);
    ASSERT_NEAR_C(sim.state[1], 0);
    ASSERT_NEAR_C(sim.state[2], std::complex<float>(0, 1));
    ASSERT_NEAR_C(sim.state[3], 0);
}