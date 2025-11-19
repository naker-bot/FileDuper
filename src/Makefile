CXX      ?= g++
CXXFLAGS ?= -std=c++20 -O2 -Wall -Wextra -pthread -DIMGUI_IMPL_OPENGL_LOADER_GLAD
LDFLAGS  ?=

# Adjust these paths if your ImGui tree is stored elsewhere.
IMGUI_DIR         ?= /home/nex/c++/lib/imgui
IMGUI_BACKENDS    ?= $(IMGUI_DIR)/backends

TARGET   ?= fileduper
SRCS      = main.cpp \
            tree_view.cpp \
            thread_pool.cpp \
            nfs_helpers.cpp \
            $(IMGUI_DIR)/imgui.cpp \
            $(IMGUI_DIR)/imgui_draw.cpp \
            $(IMGUI_DIR)/imgui_tables.cpp \
            $(IMGUI_DIR)/imgui_widgets.cpp \
            $(IMGUI_BACKENDS)/imgui_impl_glfw.cpp \
            $(IMGUI_BACKENDS)/imgui_impl_opengl3.cpp

OBJS      = $(SRCS:.cpp=.o)
INCLUDES  = -I. -I$(IMGUI_DIR) -I$(IMGUI_BACKENDS)

# glfw3 pkg-config usually drags in X11, pthread, etc.
GLFW_LIBS := $(shell pkg-config --libs glfw3 2>/dev/null)
GLFW_CXX  := $(shell pkg-config --cflags glfw3 2>/dev/null)

LDLIBS   ?= $(GLFW_LIBS) -lX11 -lXrandr -lXi -lGL -ldl -lcurl -lssl -lcrypto
CXXFLAGS += $(GLFW_CXX)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

FileDuper: $(TARGET)
	./$(TARGET)

# Backwards compatibility for older scripts that use the German verb
starten: FileDuper

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all FileDuper starten clean
