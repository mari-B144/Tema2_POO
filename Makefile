# Variabile
CXX = g++
CXXFLAGS = -Wall -std=c++17 -g
TARGET = program_cinematograf

# Identificăm automat toate fișierele .cpp
SRCS = Bilet.cpp Cinematograf.cpp main.cpp Proiectie.cpp ProiectieVIP.cpp Sala.cpp
# Transformăm lista de .cpp în listă de .o (fișiere obiect)
OBJS = $(SRCS:.cpp=.o)

# Regula principală
all: $(TARGET)

# Link-uirea obiectelor pentru a crea executabilul
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compilarea fiecărui fișier sursă în fișier obiect
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Curățarea fișierelor generate
clean:
	rm -f $(OBJS) $(TARGET)

# Regula pentru rulare
run: all
	./$(TARGET)