#!/bin/bash

valgrind --tool=callgrind --trace-children=yes ./build/flipet
