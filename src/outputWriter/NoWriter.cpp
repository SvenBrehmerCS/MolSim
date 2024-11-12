#include "NoWriter.h"

namespace outputWriter {

    NoWriter::NoWriter() = default;

    NoWriter::~NoWriter() = default;

    void NoWriter::plotParticles(
        [[maybe_unused]] const ParticleContainer& container, [[maybe_unused]] const std::string& filename, [[maybe_unused]] int iteration) {
        return;
    }

} // namespace outputWriter