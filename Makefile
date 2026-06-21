# Nombre del binario final
TARGET = lazycat

# Compilador y estándar de C++ obligatorio para std::filesystem
CXX = g++
CXXFLAGS = -std=c++17 -O3 -march=native -Wall -Wextra

# Directorios
SRC_DIR = src
INC_DIR = include

# Buscar de forma automática todos los archivos .cpp en la carpeta src/
# Excluimos temporalmente LazyTUI.cpp para poder probar la CLI pura sin stubs
SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/ConfigManager.cpp \
       $(SRC_DIR)/ModelManager.cpp \
       $(SRC_DIR)/ExecutionEngine.cpp

# Generar la lista de archivos de objeto (.o) en la raíz temporal
OBJS = $(SRCS:.cpp=.o)

# Regla principal por defecto
all: $(TARGET)

# Enlace del binario final
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -o $(TARGET) $(OBJS)

# Compilación de cada archivo fuente individual
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Limpieza de archivos basura de compilación
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)

# Forzar que estas reglas no se confundan con nombres de archivos reales
.PHONY: all clean