#include "frame_simulator.h"

#include "../benchmark_util.h"

BENCHMARK(FrameSimulator_depolarize1_100Kqubits_1Ksamples_per1000) {
    size_t num_qubits = 100 * 1000;
    size_t num_samples = 1000;
    float probability = 0.001;
    std::mt19937_64 rng(0);
    FrameSimulator sim(num_qubits, num_samples, 1, rng);

    std::vector<size_t> targets;
    for (size_t k = 0; k < num_qubits; k++) {
        targets.push_back(k);
    }

    benchmark_go([&]() { sim.DEPOLARIZE1(targets, probability); })
        .goal_millis(5)
        .show_rate("OpQubits", targets.size() * num_samples);
}

BENCHMARK(FrameSimulator_depolarize2_100Kqubits_1Ksamples_per1000) {
    size_t num_qubits = 100 * 1000;
    size_t num_samples = 1000;
    float probability = 0.001;
    std::mt19937_64 rng(0);
    FrameSimulator sim(num_qubits, num_samples, 1, rng);

    std::vector<size_t> targets;
    for (size_t k = 0; k < num_qubits; k++) {
        targets.push_back(k);
    }

    benchmark_go([&]() { sim.DEPOLARIZE2(targets, probability); })
        .goal_millis(5)
        .show_rate("OpQubits", targets.size() * num_samples);
}

BENCHMARK(FrameSimulator_hadamard_100Kqubits_1Ksamples) {
    size_t num_qubits = 100 * 1000;
    size_t num_samples = 1000;
    std::mt19937_64 rng(0);
    FrameSimulator sim(num_qubits, num_samples, 1, rng);

    std::vector<size_t> targets;
    for (size_t k = 0; k < num_qubits; k++) {
        targets.push_back(k);
    }

    benchmark_go([&]() { sim.H_XZ(targets); }).goal_millis(2).show_rate("OpQubits", targets.size() * num_samples);
}

BENCHMARK(FrameSimulator_CX_100Kqubits_1Ksamples) {
    size_t num_qubits = 100 * 1000;
    size_t num_samples = 1000;
    std::mt19937_64 rng(0);
    FrameSimulator sim(num_qubits, num_samples, 1, rng);

    std::vector<size_t> targets;
    for (size_t k = 0; k < num_qubits; k++) {
        targets.push_back(k);
    }

    benchmark_go([&]() { sim.CX(targets); }).goal_millis(2).show_rate("OpQubits", targets.size() * num_samples);
}

BENCHMARK(FrameSimulator_sample_unrotated_surface_code_d5_samples1024) {
    size_t num_samples = 1024;
    size_t distance = 5;
    auto circuit = unrotated_surface_code_circuit(distance);
    std::mt19937_64 rng(0);
    simd_bits ref(circuit.num_measurements);
    benchmark_go([&]() { FrameSimulator::sample(circuit, ref, num_samples, rng); })
        .goal_micros(100)
        .show_rate("Layers", distance * num_samples)
        .show_rate("OutBits", circuit.num_measurements * num_samples);
}

BENCHMARK(FrameSimulator_sample_unrotated_surface_code_d41_samples1024) {
    size_t num_samples = 1024;
    size_t distance = 41;
    auto circuit = unrotated_surface_code_circuit(distance);
    std::mt19937_64 rng(0);
    simd_bits ref(circuit.num_measurements);
    benchmark_go([&]() { FrameSimulator::sample(circuit, ref, num_samples, rng); })
        .goal_millis(50)
        .show_rate("Layers", distance * num_samples)
        .show_rate("OutBits", circuit.num_measurements * num_samples);
}