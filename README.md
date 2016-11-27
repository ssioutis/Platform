# Platform


CRender should contain functions for displaying data to screen only

Add lots of functions for adding technical indicators (using TA-lib, currently working on this now)

TODO:

Add CBacktest class for testing performance against a benchmark

Add CAsset class with methods for processing and containing asset data: eg stocks and bonds then eventually derivatives, etfs...

Add CStrategy class for building a strategy: Adding events, signals, orders

Add CPortfolio class to contain multiple CAsset instances and eventually optimization methods


Need to find data somehow? Interactive Brokers API would be nice. For now we can do a lot with just simulated Brownian motion.
