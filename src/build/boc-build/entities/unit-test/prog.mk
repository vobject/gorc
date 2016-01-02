NAME:=boc-build-entities-test
BIN_DIR:=$(TEST_BIN)

EXCLUDE_TEST_COVERAGE:=1

DEPENDENCIES:= \
	build/boc-build/entities \
	libs/test \

SOURCES:= \
	graph_entity_test.cpp \
	project_entity_test.cpp \
	root_entity_test.cpp \

LIBRARIES:=$(SYSTEM_LIBRARIES)