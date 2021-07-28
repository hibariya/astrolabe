.PHONY: test generate-test-data

test: export WNSEARCHDIR=../vendor/WordNet-3.0/dict
test:
			set -e; \
			for filename in $$(ls data); do \
							echo "testing: $${filename}"; \
							../dist/bin/astrolabe $$(echo $$filename | tr "-" " ") > out; \
							diff -u data/$$filename out; \
							rm out; \
			done

generate-test-data: export WNSEARCHDIR=../vendor/WordNet-3.0/dict
generate-test-data:
			for filename in $$(cat combinations); do \
							echo "generating: $${filename}"; \
							../dist/bin/astrolabe $$(echo $$filename | tr "-" " ") > data/$${filename}; \
			done
