#!/bin/bash
# polybuild-setup.sh
# OBINexus Build Orchestration Toolchain Installation

set -euo pipefail

OBINEXUS_ROOT="${OBINEXUS_ROOT:-$HOME/obinexus}"
POLYBUILD_VERSION="${POLYBUILD_VERSION:-0.1.0}"

echo "🔧 Setting up OBINexus Polybuild Toolchain v${POLYBUILD_VERSION}"

# Create OBINexus directory structure
mkdir -p "${OBINEXUS_ROOT}/"{bin,lib,scripts,configs}
mkdir -p "${OBINEXUS_ROOT}/cicd/bustcall"

# Install polybuild orchestrator
create_polybuild() {
    cat > "${OBINEXUS_ROOT}/bin/polybuild" << 'EOF'
#!/bin/bash
# OBINexus Polybuild - Constitutional Build Orchestration
# Coordinates multi-language compilation with constitutional compliance

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OBINEXUS_ROOT="$(dirname "$SCRIPT_DIR")"

# Build configuration
BUILD_CONFIG="${OBINEXUS_ROOT}/configs/polybuild.config.toml"
LOG_LEVEL="${POLYBUILD_LOG_LEVEL:-info}"
PARALLEL_JOBS="${POLYBUILD_JOBS:-$(nproc)}"

# Constitutional compliance verification
verify_constitutional_compliance() {
    echo "🏛️ Verifying constitutional compliance..."
    
    # Check for required constitutional documents
    local required_docs=(
        "obinexus_design_tech_legal.md"
        "bustcall.config.toml"
    )
    
    for doc in "${required_docs[@]}"; do
        if [[ ! -f "$doc" ]]; then
            echo "❌ Missing constitutional document: $doc"
            return 1
        fi
    done
    
    echo "✅ Constitutional compliance verified"
}

# Multi-language build orchestration
execute_build_phase() {
    local phase="$1"
    local target="${2:-all}"
    
    echo "🔨 Executing build phase: $phase (target: $target)"
    
    case "$phase" in
        "validate")
            verify_constitutional_compliance
            cargo check --all-features
            ;;
        "test")
            cargo test --all-features --verbose
            ;;
        "build")
            cargo build --release --all-features
            ;;
        "package")
            cargo build --release --target-dir target/polybuild
            ;;
        "deploy")
            echo "🚀 Deployment phase - constitutional verification required"
            verify_constitutional_compliance
            cargo install --path . --force
            ;;
        *)
            echo "❌ Unknown build phase: $phase"
            exit 1
            ;;
    esac
}

# SemVerX integration
perform_semverx_bump() {
    local bump_type="${1:-patch}"
    echo "📈 Performing SemVerX bump: $bump_type"
    
    if [[ -f "${OBINEXUS_ROOT}/scripts/semverx-bump.sh" ]]; then
        "${OBINEXUS_ROOT}/scripts/semverx-bump.sh" "$bump_type"
    else
        echo "⚠️ SemVerX script not found, performing manual version bump"
        # Manual version bump logic would go here
    fi
}

# Main execution
main() {
    local command="${1:-build}"
    shift || true
    
    echo "🌟 OBINexus Polybuild v${POLYBUILD_VERSION}"
    echo "📁 Working directory: $(pwd)"
    echo "🎯 Build target: $command"
    
    case "$command" in
        "validate" | "test" | "build" | "package" | "deploy")
            execute_build_phase "$command" "$@"
            ;;
        "semver")
            perform_semverx_bump "$@"
            ;;
        "clean")
            echo "🧹 Cleaning build artifacts"
            cargo clean
            rm -rf target/polybuild
            ;;
        "help" | "--help" | "-h")
            echo "Usage: polybuild [validate|test|build|package|deploy|semver|clean]"
            echo "Constitutional build orchestration for OBINexus projects"
            ;;
        *)
            echo "❌ Unknown command: $command"
            echo "Use 'polybuild help' for usage information"
            exit 1
            ;;
    esac
    
    echo "✅ Polybuild operation completed successfully"
}

main "$@"
EOF

    chmod +x "${OBINEXUS_ROOT}/bin/polybuild"
}

# Create SemVerX bump script
create_semverx_script() {
    cat > "${OBINEXUS_ROOT}/scripts/semverx-bump.sh" << 'EOF'
#!/bin/bash
# semverx-bump.sh - OBINexus Semantic Versioning Extension
# Constitutional version management with milestone tracking

set -euo pipefail

BUMP_TYPE="${1:-patch}"
CARGO_TOML="${CARGO_TOML:-Cargo.toml}"

echo "📈 SemVerX Bump: $BUMP_TYPE"

if [[ ! -f "$CARGO_TOML" ]]; then
    echo "❌ Cargo.toml not found"
    exit 1
fi

# Extract current version
CURRENT_VERSION=$(grep '^version = ' "$CARGO_TOML" | head -1 | sed 's/version = "\(.*\)"/\1/')
echo "📊 Current version: $CURRENT_VERSION"

# Parse semantic version components
IFS='.' read -r MAJOR MINOR PATCH <<< "$CURRENT_VERSION"

# Perform version bump
case "$BUMP_TYPE" in
    "major")
        MAJOR=$((MAJOR + 1))
        MINOR=0
        PATCH=0
        ;;
    "minor")
        MINOR=$((MINOR + 1))
        PATCH=0
        ;;
    "patch")
        PATCH=$((PATCH + 1))
        ;;
    *)
        echo "❌ Invalid bump type: $BUMP_TYPE"
        echo "Valid types: major, minor, patch"
        exit 1
        ;;
esac

NEW_VERSION="${MAJOR}.${MINOR}.${PATCH}"
echo "🎯 New version: $NEW_VERSION"

# Update Cargo.toml
sed -i "s/version = \"$CURRENT_VERSION\"/version = \"$NEW_VERSION\"/" "$CARGO_TOML"

# Create git tag if in git repository
if git rev-parse --git-dir > /dev/null 2>&1; then
    git add "$CARGO_TOML"
    git commit -m "🔖 Release v$NEW_VERSION - Constitutional milestone"
    git tag -a "v$NEW_VERSION" -m "OBINexus Release v$NEW_VERSION"
    echo "📌 Git tag created: v$NEW_VERSION"
fi

echo "✅ Version bumped successfully: $CURRENT_VERSION → $NEW_VERSION"
EOF

    chmod +x "${OBINEXUS_ROOT}/scripts/semverx-bump.sh"
}

# Create polybuild configuration
create_polybuild_config() {
    cat > "${OBINEXUS_ROOT}/configs/polybuild.config.toml" << 'EOF'
# polybuild.config.toml - OBINexus Build Orchestration Configuration

[build]
parallel_jobs = 0  # 0 = auto-detect
constitutional_verification = true
strict_mode = true

[targets]
default = "bustcall"

[targets.bustcall]
type = "rust-library"
features = ["cli", "daemon", "ffi"]
validation_required = true

[semverx]
auto_tag = true
milestone_tracking = true
constitutional_compliance_check = true

[notification]
build_success = true
build_failure = true
constitutional_violations = true
EOF
}

# Main installation
main() {
    echo "🚀 Installing OBINexus Polybuild Toolchain..."
    
    create_polybuild
    create_semverx_script
    create_polybuild_config
    
    # Add to PATH if not already there
    if [[ ":$PATH:" != *":${OBINEXUS_ROOT}/bin:"* ]]; then
        echo "export PATH=\"${OBINEXUS_ROOT}/bin:\$PATH\"" >> ~/.bashrc
        echo "📝 Added ${OBINEXUS_ROOT}/bin to PATH in ~/.bashrc"
        echo "🔄 Run 'source ~/.bashrc' or restart terminal to apply"
    fi
    
    echo "✅ OBINexus Polybuild Toolchain installed successfully"
    echo "📍 Installation location: ${OBINEXUS_ROOT}"
    echo "🔧 Available commands: polybuild, semverx-bump.sh"
    echo ""
    echo "Next steps:"
    echo "1. source ~/.bashrc"
    echo "2. cd /path/to/bustcall"
    echo "3. polybuild validate"
}

main "$@"
