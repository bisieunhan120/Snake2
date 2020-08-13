#include <stdio.h>
#include "..\SDL2-2.0.9\include\SDL.h"

#ifdef _MSC_VER // tham số liên kết với Visual C++
# pragma comment (lib, "sdl2main.lib")
# pragma comment (lib, "sdl2.lib")
# pragma comment (linker, "/entry:\"mainCRTStartup\"" )
# pragma comment (linker, "/subsystem:WINDOWS")
#endif
#define WWIDTH 320 // chiều rộng cửa sổ
#define WHEIGHT 240 // chiều cao cửa sổ
#define BLK_SIZE 16 // kích thước hình vuông
SDL_Window* wnd; // con trỏ cửa sổ SDL
SDL_Renderer* rend; // con trỏ đối tượng dựng hình
int x = WWIDTH / 2, vx = 2; // tọa độ và vận tốc di chuyển
int y = WHEIGHT / 2, vy = 2; // ban đầu của hình vuông
void block(int x, int y) // vẽ hình vuông 16x16 tại tọa độ (x, y)
{ // sử dụng màu trắng
	SDL_Rect r = { x, y, BLK_SIZE, BLK_SIZE };
	SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
	SDL_RenderFillRect(rend, &r);
}
void init(int vsync) // khởi tạo môi trường đồ họa SDL
{ // vsync = 1 nếu muốn sử dụng V-Sync
	SDL_Init(SDL_INIT_VIDEO); // khởi tạo hệ thống đồ họa
							  // tạo cửa sổ tại vị trí (100, 100), kích thước 640x480.
	wnd = SDL_CreateWindow("FPS", 200, 200, WWIDTH, WHEIGHT, SDL_WINDOW_SHOWN);
	// tạo đối tượng dựng hình 2D, kiểm tra việc kích hoạt V-Sync
	rend = SDL_CreateRenderer(wnd, -1,
		SDL_RENDERER_ACCELERATED |
		(vsync ? SDL_RENDERER_PRESENTVSYNC : 0));
}
int event() // xử lý sự kiện
{
	SDL_Event e; // biến lưu trữ sự kiện
	int running = 1; // biến kiểm soát vòng lặp
	while (SDL_PollEvent(&e) != 0) // lấy 1 sự kiện khỏi hàng đợi, nếu có
	{
		// nếu là phím nhấn, kiểm tra có phải ESC được nhấn hay không
		if (e.type == SDL_KEYDOWN)
			if (e.key.keysym.sym == SDLK_ESCAPE)
				running = 0; // nếu đúng, ta yêu cầu đóng chương trình
							 // nếu là sự kiện đóng cửa sổ (nhấn vào nút [X])
		if (e.type == SDL_QUIT)
			running = 0; // ta cũng yêu cầu đóng chương trình
	}
	return running;
}
void update() // cập nhật trạng thái chương trình
{
	x += vx; y += vy; // di chuyển hình vuông
					  // nảy ngược lại nếu chạm các cạnh cửa sổ
	if (x < 0 || x > WWIDTH - BLK_SIZE) vx = -vx;
	if (y < 0 || y > WHEIGHT - BLK_SIZE) vy = -vy;
}
void draw() // xử lý các thao tác dựng hình
{
	// xóa toàn bộ framebuffer về màu đen
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
	SDL_RenderClear(rend);
	block(x, y); // vẽ hình vuông tại vị trí hiện hành
	SDL_RenderPresent(rend); // copy framebuffer -> screen
}
void done() // dọn dẹp ứng dụng
{
	SDL_DestroyRenderer(rend); // hủy đối tượng ngữ cảnh dựng hình
	SDL_DestroyWindow(wnd); // hủy cửa sổ SDL
	SDL_Quit(); // chấm dứt phiên làm việc SDL
}
int main(int argc, char** argv)
{
	int timerBegin, frameCount; // bộ đếm FPS
	float fps; // giá trị frame-per-second
	int running = 1; // biến kiểm soát vòng lặp
	char buf[64]; // buffer tạm để hiển thị nội dung FPS
	init(1); // khởi tạo ứng dụng với VSync bật
	frameCount = 0; // bắt đầu đếm số frame đã dựng
	timerBegin = SDL_GetTicks();// lấy thời gian bắt đầu vòng lặp
	while (running)
	{
		running = event(); // xử lý sự kiện trên cửa sổ
		update(); // cập nhật trạng thái ứng dụng
		draw(); // dựng hình
		frameCount++; // đã dựng xong 1 frame
					  // tính số FPS
		fps = frameCount / ((SDL_GetTicks() - timerBegin) / 1000.0f);
		// và hiển thị lên tiêu đề cửa sổ
		sprintf_s(buf, "FPS=%0.2f", fps);
		SDL_SetWindowTitle(wnd, buf);
	}
	done(); // kết thúc ứng dụng
	return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
