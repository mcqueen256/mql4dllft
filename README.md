# MQL4 DLL Function Translator (mql4dllft)
An MQL4 API wrapper moves the operational environment from the EX4 (in the client terminal) to the DLL.

# Project Goals
There are a number of goals for this project which are:
* Moving the runtime environment to that which has access to C++11;
* Creating a structured workflow for developing and deploying Experts and Indicators; and
* Allowing indicators to access the internal buffers used by experts

# environments
The MQL4 language is advertised to be a C++ equivalent, however, lacks nearly all the advanced features of the language and some of the basic features (e.g. memory allocation). To enable traders and programers to take full advantage of both the MQL4 environment and the C++ language (specifically C++11) this project is dedicated to developing a bridge between the two environments so that the events ([OnInit](https://docs.mql4.com/basis/function/events), [OnTick](https://docs.mql4.com/basis/function/events)) are run in the C++11 (DLL) environment with access to the MQL4 API in the DLL.