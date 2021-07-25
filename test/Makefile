.PHONY: test generate-test-data

test:
			 for filename in $$(ls data); do \
							 echo "testing: $${filename}"; \
							 ../dist/bin/astrolabe $$(echo $$filename | tr "-" " ") > out; \
							 diff -u data/$$filename out; \
							 rm out; \
			 done

generate-test-data:
			 for filename in $$(cat combinations); do \
							 echo "generating: $${filename}"; \
							 ../dist/bin/astrolabe $$(echo $$filename | tr "-" " ") > data/$${filename}; \
			 done

