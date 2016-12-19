# Platform


GUI handled by Qt

Plotting/graphing handled by Qwt

Classes:

CTradingPlatform: handles GUI layout and initialization

CTechnical: implements technical indicators

CStrategy: Handles strategy building and execution

TODO:

Add CAsset class with methods for processing and containing asset data: eg stocks and bonds then eventually derivatives, etfs...

Add CPortfolio class to contain multiple CAsset instances and eventually optimization methods


Need to find data somehow? Interactive Brokers API would be nice. For now we can do a lot with just simulated Brownian motion.
