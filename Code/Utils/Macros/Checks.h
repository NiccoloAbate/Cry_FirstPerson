#pragma once

#define LAUNCHER_CHECK if(gEnv->isEditor()) return;

#define ONCE(Code)							\
static bool _bONCE = false;					\
if(!_bONCE) {Code; _bONCE = true;}			\

