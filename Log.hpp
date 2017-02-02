#pragma once

#include <stdexcept>
#include <iostream>

#include "incgraphics.h"

bool restart_gl_log();
bool gl_log(const char* message, ...);
bool gl_log_err(const char* message, ...);
void glfw_error_callback(int error, const char* description);
void log_gl_params();
const char* GL_type_to_string(GLenum type);

#define STR(x) #x
#define TOSTR(x) STR(x)
#define ERROR(format) fprintf(stderr, "error: " format "\n");

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define CODE_LOCATION "\033[1m" __FILE__ "\033[0m::\033[93m" TOSTRING(__LINE__) "\033[0m"
#define CONDITION_TOSTR(CONDITION) " ( \033[1;4m" STRINGIFY(CONDITION) "\033[0m )"

#define ASSERT(CONDITION) \
	if(!(CONDITION)) { \
		throw std::runtime_error("\033[1;91merror\033[0m at " CODE_LOCATION CONDITION_TOSTR(CONDITION)); \
	}

#define GLERROR { GLenum ret = glGetError(); if(ret != GL_NO_ERROR) { describe_error(ret); std::cerr << ret << std::endl; ASSERT(ret == GL_NO_ERROR); } };
void describe_error(GLenum code);
