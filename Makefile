format:
	find ez/src -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
	find examples -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
