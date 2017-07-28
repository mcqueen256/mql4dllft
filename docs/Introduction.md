# Introduction

The field of automated market trading there are an uncountable number of tools that offer so much support for developing autonomous trading programs (bots). Every tool offers some advantageous feature that makes it more attractive to use for particular situations. For instance, MetaTrader offers the ability to write technical indicators to predict market behaviour and its own language to write autonomous trade application. CTrader is very similar to MetaTrader4, however, offers C/C# as its choice of language. MetaTrader4 (MT4) also offers optimisation tools that test the behaviour of bots over its inputs for maximum profit. There are other tools out there like JavaScript/cloud alternatives to MT4 and web APIs that use URL web calls to perform market actions. Price over the tool may also be a factor in the usability of it (APIs are usually come at a monstrous cost).

I (the developer Nicholas Buckeridge [McQueen256]) have a few problems with this (yes, I am sharing my personal view here.) The problem with these tools are there is no cheap, easy to develop/debug tool with nearly all of the features that make automated trading bots highly effective. What makes a robot effective?

* Back testing.
* Optimisation.
* Access to a full language (C++11 in this case).
* A joint framework between multiple indicators and a single trade robot.
* The easy of configuring such a framework.
* Access to a powerful API.
* Software practices like unit testing, code quality scanning and continuous integration.
* Easy deployment.
* And, of course, the robots algorithm.

There are arguable more points that make the development of a trading robot but these are the main focuses of this repository.

# Framework Architecture

## The Pipeline

This *MQLDLLFT framework* heavily focuses on the development pipeline to allow for rapid, productive development and prototyping.

configure => Initial project setup => write bot algorithm => DLL and MT4 script compilation => testing => deployment

This *MQLDLLFT framework* depends on the MetaTrader4 platform which provides the chart/quote/API/backtesting/optimising access. To enable the programmer to write in _C++11_ the program will make use of a DLL. With 




