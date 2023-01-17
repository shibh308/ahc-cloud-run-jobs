cargo build --release --bin tester --target-dir ./tools/target --manifest-path tools/Cargo.toml
cp ./tools/target/release/tester ../upload_files