// Adder.cpp
#include "Adder.h"

JNIEXPORT jint JNICALL Java_Adder_add(JNIEnv* env, jobject obj, jint a, jint b) {
    return a + b;
}
