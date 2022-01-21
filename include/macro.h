// macro stringizing
#define str_temp(x) #x
#define str(x) str_temp(x)

// strlen() for string constant
#define STRLEN(CONST_STR) (sizeof(CONST_STR) - 1)

// calculate the length of an array
#define ARRLEN(arr) (int)(sizeof(arr) / sizeof(arr[0]))

// macro concatenation
#define concat_temp(x, y) x ## y
#define concat(x, y) concat_temp(x, y)

// vga macros
#define VGA_POS_EDGE(constr) (concat(vga_, constr) == 1 && concat(vga_pre_, constr) == 0)
#define VGA_NEG_EDGE(constr) (concat(vga_, constr) == 0 && concat(vga_pre_, constr) == 1)
