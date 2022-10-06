# Lexical-Analysis
將讀入的資料切成token，找出對應的table與位置。
# 開發平台:
OS : macOS Big Sur CPU : M1
RAM : 8G
# 使用開發環境:
Xcode v12.5.1 (12E507) 
# 使用的程式語言:
C++
# 組合語言: SIC/XE
#程式功能: 
將讀入的資料切成token，找出對應的table與位置。
#程式流程: 
*首先輸入1，若是要結束程式則輸入0，
*再輸入檔名ex: input。
*建立struct Tabledata，內容有string instuction(存指令), int table(table), int location(在table之位置)。
*將4個table的內容讀進我所創立的vector <Tabledata> table1~4中，開啟測試檔後，將內容包括空白逐一由字元 讀出並輸出、印出，在遇到到whitespace或是delimiter前， 先將每個字元由字串儲存起來，遇到whitespace或 delimiter後，若字串不為空字串就將字串去4個table vector中尋找是否存在，若存在就將所在的table與位置存 在最終要輸出的陣列，如果不存在就先判斷是否為是 integer或是字串，若都不是就為symbol，分別為table6,7,5 ，並在存入時將字串的每個字元ascii code相加後mod 100 取餘數，若此字串已存在或是此位置為空，就將所在的 table與位置存在最終要輸出的陣列，並且將餘數的hash位 置存入table array中，若此位置已有字串且不相等，就一直 到找到空的位置再將所在的table與位置存在最終要輸出 的陣列，遇到換行時就將最終陣列中尚未輸出、印出的資 料輸出、印出，在讀入下一字元以此類推。其中，SIC/XE中 要特別處理的部分為ex: X’F1’, C’EOF’兩個部分，如果是 X’F1的狀況就忽略X然後再將‘去table中尋找後存入vector ，接著處理F1用integer的方式去處理; C’EOF’的部分也是 忽略C後處理‘，再將EOF以字串的方式去處理。
