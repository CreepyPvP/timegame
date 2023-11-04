vim.keymap.set("n", "<", "<cmd>tabnew term://cd build && make && ./timegame<CR>");
vim.keymap.set("n", ">", "<cmd>tabnew term://cd build && make && gdb ./timegame<CR>");
