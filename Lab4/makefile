# Компилятор и флаги
CXX = gcc
CXXFLAGS = -Wall -Wextra -O2 -Iinclude

# Библиотеки
LIBS = libGCF_Euclid.dll libGCF_Naive.dll libPi_Leibniz.dll libPi_Wallis.dll

# Тестовые программы
APP1 = TestProgram1.exe
APP2 = TestProgram2.exe

# Исходные файлы для библиотек
GCF_EUCLID_SRC = src/GCF_Euclid.c
GCF_NAIVE_SRC = src/GCF_Naive.c
PI_LEIBNIZ_SRC = src/Pi_Leibniz.c
PI_WALLIS_SRC = src/Pi_Wallis.c

# Исходные файлы для тестовых программ
TEST1_SRC = test/TestProgram1.c
TEST2_SRC = test/TestProgram2.c

# Цели
all: $(LIBS) $(APP1) $(APP2)

# Правила для создания библиотек
libGCF_Euclid.dll: $(GCF_EUCLID_SRC)
	$(CXX) $(CXXFLAGS) -shared -o $@ $<

libGCF_Naive.dll: $(GCF_NAIVE_SRC)
	$(CXX) $(CXXFLAGS) -shared -o $@ $<

libPi_Leibniz.dll: $(PI_LEIBNIZ_SRC)
	$(CXX) $(CXXFLAGS) -shared -o $@ $<

libPi_Wallis.dll: $(PI_WALLIS_SRC)
	$(CXX) $(CXXFLAGS) -shared -o $@ $<

# Правила для создания тестовых программ
$(APP1): $(TEST1_SRC) libGCF_Euclid.dll libPi_Leibniz.dll
	$(CXX) $(CXXFLAGS) -o $@ $< -L. -lGCF_Euclid -lPi_Leibniz

$(APP2): $(TEST2_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $< -L.

# Удаление сборок
clean:
	del $(LIBS) $(APP1) $(APP2)
