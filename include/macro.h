// macro concatenation
#define concat_temp(x, y) x ## y
#define concat(x, y) concat_temp(x, y)

// vga macros
#define VGA_POS_EDGE(constr) (concat(vga_, constr) == 1 && concat(vga_pre_, constr) == 0)
#define VGA_NEG_EDGE(constr) (concat(vga_, constr) == 0 && concat(vga_pre_, constr) == 1)

#if !defined(likely)
#define likely(cond)   __builtin_expect(cond, 1)
#define unlikely(cond) __builtin_expect(cond, 0)
#endif
