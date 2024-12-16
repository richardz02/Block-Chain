# Block_Chain
This is a simple blockchain project.

Each block will consist of the following:
- Block index
- Previous block hash
- Timestamp
- Data
- Hash of current block

The order in which the serialized bytes are added to buffer are:
1. Block index
2. Timestamp
3. Data
4. Previous hash
Data types such as integers and timestamp are converted into little endian format 
