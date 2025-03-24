format:
	find ezUI/src -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i