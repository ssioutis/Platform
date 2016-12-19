# Platform


GUI handled by Qt

Plotting/graphing handled by Qwt

**Install Qt:**

1) Download Qt creator here https://www.qt.io/download-open-source/

2) When installing, at the Select Components screen, under Qt 5.7, check Data Visualization and Charts

**Install Qwt:**

1) Get zip file

2) Extract file at C:\qwt-6.1.3. *Make sure you can access C:\qwt-6.1.3\features*

3) Edit environment variables:

3a) Under "user variables" make 2 new variables. 1st with name QMAKEFEATURES and value C:\qwt-6.1.3\features. 2nd with name QT_PLUGIN_PATH and value C:\qwt-6.1.3\plugins.

3b) Click PATH and edit. Add C:\qwt-6.1.3\lib

Should work fine from here.
	
**Classes:**

1) CTradingPlatform: handles GUI layout and initialization

2) CTechnical: implements technical indicators

3) CStrategy: Handles strategy building and execution

**TODO:**

1) Add CAsset class with methods for processing and containing asset data: eg stocks and bonds then eventually derivatives, etfs...

2) Add CPortfolio class to contain multiple CAsset instances and eventually optimization methods


Need to find data somehow? Interactive Brokers API would be nice. For now we can do a lot with just simulated Brownian motion.
