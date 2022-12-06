#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <d3dx9core.h>
#include <d3dx9tex.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#include "color.h"
#include <vector>
#include <string>

//custom vertex composures
namespace fvf
{
	auto transformed = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
	auto transformed_uv = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
}

//custom vertex structures
namespace v
{
	struct transformed {
		float x, y, z, rhw;
		DWORD color;
	};

	struct transformed_uv
	{
		float x, y, z, rhw;
		DWORD color;
		float u, v;
	};
}
struct dim_t
{
	int w, h;
};

//fonts
namespace f
{
	std::vector<ID3DXFont*> fonts;
	ID3DXFont* tahoma = nullptr;
	ID3DXFont* tahoma_1 = nullptr;
	ID3DXFont* tahoma_2 = nullptr;
	ID3DXFont* tahoma_3 = nullptr;

	void make_font(IDirect3DDevice9* m_pdevice, ID3DXFont** out, const char* name, int size, int weight, bool antialias = false)
	{
		auto zz = D3DXCreateFontA(m_pdevice, size, 0, weight, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, antialias ? ANTIALIASED_QUALITY : DEFAULT_QUALITY, FIXED_PITCH | FF_DONTCARE, name, out);
		fonts.push_back(*out);
	}

	void startup(IDirect3DDevice9* m_pdevice)
	{
		//ProggyClean
		make_font(m_pdevice, &tahoma, "Tahoma", 12, FW_NORMAL, 0);
		make_font(m_pdevice, &tahoma_1, "Tahoma", 12, FW_NORMAL, 0);
		make_font(m_pdevice, &tahoma_2, "Tahoma", 14, FW_NORMAL, 0);
		make_font(m_pdevice, &tahoma_3, "Tahoma", 18, FW_NORMAL, 0);
	}
}

//for loading resource files
class ctexture
{
public:
	//ctexture(IDirect3DDevice9* m_pdevice, int width, int height, void* data, unsigned int size)
	//{
	//	HRESULT ret = D3DXCreateSprite(m_pdevice, &sprite);
	//	if (ret != D3D_OK) return;
	//
	//	//create a temp texture
	//	LPDIRECT3DTEXTURE9 temp_texture;
	//
	//	ret = D3DXCreateTextureFromFileInMemoryEx(m_pdevice, data, size, width, height, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &texture);
	//	if (ret != D3D_OK) return;
	//}

	bool mount_image(IDirect3DDevice9* m_pdevice, const char* filename, D3DCOLOR colorkey = 0)
	{
		HRESULT ret = D3DXGetImageInfoFromFile(filename, &img_info);
		if (ret != D3D_OK) return false;

		ret = D3DXCreateSprite(m_pdevice, &sprite);
		if (ret != D3D_OK)  return false;

		ret = D3DXCreateTextureFromFileEx(m_pdevice, filename, img_info.Width, img_info.Height, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, colorkey, NULL, NULL, &texture);
		if (ret != D3D_OK) return false;

		center.x = (float)img_info.Width / 2;
		center.y = (float)img_info.Height / 2;

		ready_state = true;
		return true;
	}

	bool ready()
	{
		return ready_state;
	}

	void color_shift(D3DCOLOR newcolor)
	{
		color = newcolor;
	}

	void color_shift(const color_t& newcolor, int alpha)
	{
		color = color2dx(newcolor, alpha);
	}

	void render()
	{
		sprite->Begin(0);
		sprite->Draw(texture, NULL, NULL, NULL, color);
		sprite->End();
	}

	//info about the image loaded
	D3DXIMAGE_INFO img_info = { 0 };

	//center point
	D3DXVECTOR2 center = { 0,0 };
	
	//color
	D3DCOLOR color = 0xFFFFFFFF;

	//texture data
	LPD3DXSPRITE sprite = nullptr;
	LPDIRECT3DTEXTURE9 texture = nullptr;

	//ready state
	bool ready_state = false;
};

//main functions
namespace d
{
	IDirect3D9Ex* m_pd3d = nullptr;
	IDirect3DDevice9Ex* m_pdevice = nullptr;
	ID3DXLine* m_pline = nullptr;

	//cursor surface
	IDirect3DSurface9* m_pcursorsurface = nullptr;

	//current font in use
	ID3DXFont* m_pfont = nullptr;

	//parameters for the device
	D3DPRESENT_PARAMETERS m_presentparams;

	//centering text based on font size
	bool m_center_text_y = false;

	//outline for text
	bool m_textoutline = false;
	//color for text outline
	DWORD m_textoutline_color = color2dx(color::black, 140);

	void set_default_states()
	{
		m_pdevice->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
		m_pdevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		//for overlay
		//m_pdevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		//m_pdevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//for normal
		m_pdevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pdevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		m_pdevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, true);
		m_pdevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pdevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pdevice->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
		m_pdevice->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
	}

	void on_resize(int width, int height)
	{
		if (width && height)
		{
			m_presentparams.BackBufferWidth = width;
			m_presentparams.BackBufferHeight = height;
			if (m_pdevice) {
				m_pdevice->Reset(&m_presentparams);
			}
		}
	}

	bool startup(HWND hwnd)
	{
		HRESULT ret = Direct3DCreate9Ex(D3D_SDK_VERSION, &m_pd3d);
		if (FAILED(ret)) return false;

		ZeroMemory(&m_presentparams, sizeof m_presentparams);
		m_presentparams.hDeviceWindow = hwnd;
		m_presentparams.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_presentparams.BackBufferFormat = D3DFMT_A8R8G8B8;
		m_presentparams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		m_presentparams.Windowed = 1;
		m_presentparams.MultiSampleType = D3DMULTISAMPLE_TYPE::D3DMULTISAMPLE_4_SAMPLES;

		ret = m_pd3d->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_presentparams, 0, &m_pdevice);
		if (FAILED(ret)) return false;

		ret = D3DXCreateLine(m_pdevice, &m_pline);
		if (FAILED(ret)) return false;

		//set up fonts
		f::startup(m_pdevice);

		set_default_states();

		return true;
	}

	void line(float x, float y, float x1, float y1, const color_t& color, float alpha, float width, bool antialias = false)
	{
		static D3DXVECTOR2 points[2];
		points[0] = { x,y };
		points[1] = { x1,y1 };

		m_pline->SetWidth(width);
		m_pline->SetAntialias(antialias);

		m_pline->Begin();
		m_pline->Draw(points, 2, color2dx(color, alpha));
		m_pline->End();
	}

	void rect(float x, float y, float x1, float y1, const color_t& color, float alpha, bool fill = false)
	{
		auto dxcolor = color2dx(color, alpha);

		if (!fill)
		{
			x1--;
			y1--;
		}

		std::vector<v::transformed> vertices = {
			{x,y,1.f,1.f,dxcolor},
			{x1,y,1.f,1.f,dxcolor},
			{x1,y1,1.f,1.f,dxcolor},
			{x,y1,1.f,1.f,dxcolor},
			{x,y,1.f,1.f,dxcolor}
		};

		m_pdevice->SetFVF(fvf::transformed);
		m_pdevice->DrawPrimitiveUP(fill ? D3DPT_TRIANGLESTRIP : D3DPT_LINESTRIP, fill ? 3 : 4, &vertices[0], sizeof v::transformed);
	}

	void rect1(float x, float y, float w, float h, const color_t& color, float alpha, bool fill = false)
	{
		auto dxcolor = color2dx(color, alpha);
		
		if (!fill)
		{
			w--;
			h--;
		}

		std::vector<v::transformed> vertices = {
			{x,y,0.f,1.f,dxcolor},
			{x + w,y,0.f,1.f,dxcolor},
			{x + w,y + h,0.f,1.f,dxcolor},
			{x,y + h,0.f,1.f,dxcolor},
			{x,y,0.f,1.f,dxcolor}
		};

		m_pdevice->SetFVF(fvf::transformed);
		m_pdevice->DrawPrimitiveUP(fill ? D3DPT_TRIANGLESTRIP : D3DPT_LINESTRIP, fill ? 3 : 4, &vertices[0], sizeof v::transformed);
	}

	void rect_center(float x, float y, float size, const color_t& color, float alpha, bool fill = false) {
		rect(x - size, y - size, x + size, y + size, color, alpha, fill);
	}

	void gradient_h(float x, float y, float x1, float y1, const gradient_t& color, int alpha, int alpha2)
	{
		auto dxcolor0 = color2dx(color.top, alpha);
		auto dxcolor1 = color2dx(color.bottom, alpha2);

		std::vector<v::transformed> vertices = {
			{x,y1,1,1,dxcolor0},
			{x,y,1,1,dxcolor0},
			{x1,y1,1,1,dxcolor1},
			{x1,y,1,1,dxcolor1}
		};

		m_pdevice->SetFVF(fvf::transformed);
		m_pdevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &vertices[0], sizeof v::transformed);
	}

	void gradient_h1(float x, float y, float w, float h, const gradient_t& color, int alpha, int alpha2)
	{
		auto dxcolor0 = color2dx(color.top, alpha);
		auto dxcolor1 = color2dx(color.bottom, alpha2);

		std::vector<v::transformed> vertices = {
			{x,y + h,1,1,dxcolor0},
			{x,y,1,1,dxcolor0},
			{x + w,y + h,1,1,dxcolor1},
			{x + w,y,1,1,dxcolor1}
		};

		m_pdevice->SetFVF(fvf::transformed);
		m_pdevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &vertices[0], sizeof v::transformed);
	}
	
	void gradient_v(float x, float y, float x1, float y1, const gradient_t& color, int alpha, int alpha2)
	{
		auto dxcolor0 = color2dx(color.top, alpha);
		auto dxcolor1 = color2dx(color.bottom, alpha2);

		std::vector<v::transformed> vertices = {
			{x,y,1,1,dxcolor0},
			{x1,y,1,1,dxcolor0},
			{x,y1,1,1,dxcolor1},
			{x1,y1,1,1,dxcolor1}
		};

		m_pdevice->SetFVF(fvf::transformed);
		m_pdevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &vertices[0], sizeof v::transformed);
	}

	void gradient_v1(float x, float y, float w, float h, const gradient_t& color, int alpha, int alpha2)
	{
		auto dxcolor0 = color2dx(color.top, alpha);
		auto dxcolor1 = color2dx(color.bottom, alpha2);

		std::vector<v::transformed> vertices = {
			{x,y,1,1,dxcolor0},
			{x + w,y,1,1,dxcolor0},
			{x,y + h,1,1,dxcolor1},
			{x + w,y + h,1,1,dxcolor1}
		};

		m_pdevice->SetFVF(fvf::transformed);
		m_pdevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &vertices[0], sizeof v::transformed);
	}

	void gradient_rect_center_h(float x, float y, float size, const gradient_t& color, int alpha, int alpha2) {
		gradient_h(x - size, y - size, x + size, y + size, color, alpha, alpha2);
	}

	void gradient_rect_center_v(float x, float y, float size, const gradient_t& color, int alpha, int alpha2) {
		gradient_v(x - size, y - size, x + size, y + size, color, alpha, alpha2);
	}

	void circle(float x, float y, float size, int resolution, const color_t& color, int alpha, bool fill = false)
	{
		std::vector<v::transformed> vertices(resolution + 2);

		auto dxcolor = color2dx(color, alpha);

		for (int i = 0; i < resolution + 2; ++i)
		{
			vertices[i].x = x - size * cos(i * (2 * D3DX_PI / resolution));

			vertices[i].y = y - size * sin(i * (2 * D3DX_PI / resolution));
			vertices[i].color = dxcolor;
			vertices[i].z = 1.f;
			vertices[i].rhw = 1.f;
		}

		m_pdevice->SetFVF(fvf::transformed);
		m_pdevice->DrawPrimitiveUP(fill ? D3DPT_TRIANGLEFAN : D3DPT_LINESTRIP, resolution, &vertices[0], sizeof v::transformed);
	}

	RECT get_clip()
	{
		RECT rect;
		m_pdevice->GetScissorRect(&rect);
		return rect;
	}

	void set_clip(int x, int y, int x1, int y1)
	{
		RECT clip = { x,y,x,y };
		m_pdevice->SetScissorRect(&clip);
	}

	void set_clip1(int x, int y, int w, int h)
	{
		RECT clip = { x,y,x + w,y + h };
		m_pdevice->SetScissorRect(&clip);
	}

	void set_clip(const RECT& rect)
	{
		m_pdevice->SetScissorRect(&rect);
	}

	void antialias(bool state)
	{
		m_pdevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, state);
		m_pdevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, state);
	}

	void set_font(ID3DXFont* newfont)
	{
		m_pfont = newfont;
	}

	void set_text_outline_color(DWORD color)
	{
		m_textoutline_color = color;
	}

	void set_text_outline(bool state)
	{
		m_textoutline = state;
	}

	void set_text_vertical_centering(bool state) {
		m_center_text_y = state;
	}

	dim_t get_string_dim_a(const char* string) {

		RECT rect = { 0 };
		m_pfont->DrawTextA(NULL, string, -1, &rect, DT_CALCRECT, 0);
		return { rect.right - rect.left,rect.bottom - rect.top };
	}

	dim_t get_string_dim_w(const wchar_t* string) {

		RECT rect = { 0 };
		m_pfont->DrawTextW(NULL, string, -1, &rect, DT_CALCRECT, 0);
		return { rect.right - rect.left,rect.bottom - rect.top };
	}

	//DT_LEFT - DT_CENTER - DT_RIGHT
	void string_w(float x, float y, DWORD flags, const color_t& color, int alpha, const wchar_t* string, ...)
	{
		RECT r = { x,y,x,y };
		static wchar_t buffer[256] = { 0 };

		va_list args;
		va_start(args, string);
		vswprintf(buffer, sizeof(buffer) / 2, string, args);
		va_end(args);

		if (m_center_text_y)
		{
			auto dim = get_string_dim_w(buffer);
			r.top -= dim.h / 2;
			r.bottom -= dim.h / 2;
		}

		//outline
		m_textoutline = false;
		if (m_textoutline)
		{
			RECT r1;
			r1 = { r.left - 1, r.top - 1, r.right, r.bottom };
			m_pfont->DrawTextW(NULL, buffer, -1, &r1, flags | DT_NOCLIP, m_textoutline_color);
			r1 = { r.left - 0, r.top - 1, r.right, r.bottom };
			m_pfont->DrawTextW(NULL, buffer, -1, &r1, flags | DT_NOCLIP, m_textoutline_color);
			r1 = { r.left + 1, r.top - 1, r.right, r.bottom };
			m_pfont->DrawTextW(NULL, buffer, -1, &r1, flags | DT_NOCLIP, m_textoutline_color);
			r1 = { r.left + 1, r.top - 0, r.right, r.bottom };
			m_pfont->DrawTextW(NULL, buffer, -1, &r1, flags | DT_NOCLIP, m_textoutline_color);
			r1 = { r.left + 1, r.top + 1, r.right, r.bottom };
			m_pfont->DrawTextW(NULL, buffer, -1, &r1, flags | DT_NOCLIP, m_textoutline_color);
			r1 = { r.left + 0, r.top + 1, r.right, r.bottom };
			m_pfont->DrawTextW(NULL, buffer, -1, &r1, flags | DT_NOCLIP, m_textoutline_color);
			r1 = { r.left - 1, r.top + 1, r.right, r.bottom };
			m_pfont->DrawTextW(NULL, buffer, -1, &r1, flags | DT_NOCLIP, m_textoutline_color);
			r1 = { r.left - 1, r.top - 0, r.right, r.bottom };
			m_pfont->DrawTextW(NULL, buffer, -1, &r1, flags | DT_NOCLIP, m_textoutline_color);
		}

		//main text
		auto clr = color2dx(color, alpha);
		m_pfont->DrawTextW(NULL, buffer, -1, &r, flags | DT_NOCLIP, clr);
	}

	//DT_LEFT - DT_CENTER - DT_RIGHT
	void string_a(float x, float y, DWORD flags, const color_t& color, int alpha, const char* string, ...)
	{
		RECT r = { x,y,x,y };
		static char buffer[256] = { 0 };

		va_list arg_list;
		va_start(arg_list, string);
		vsnprintf(buffer, sizeof(buffer), string, arg_list);
		va_end(arg_list);

		if (m_center_text_y)
		{
			auto dim = get_string_dim_a(buffer);
			r.top -= dim.h / 2;
			r.bottom -= dim.h / 2;
		}

		//outline
		m_textoutline = false;
		if (m_textoutline)
		{
			RECT r1;
			r1 = { r.left - 1, r.top - 1, r.right, r.bottom };
			m_pfont->DrawTextA(NULL, buffer, -1, &r1, flags | DT_NOCLIP, m_textoutline_color);
			r1 = { r.left - 0, r.top - 1, r.right, r.bottom };
			m_pfont->DrawTextA(NULL, buffer, -1, &r1, flags | DT_NOCLIP, m_textoutline_color);
			r1 = { r.left + 1, r.top - 1, r.right, r.bottom };
			m_pfont->DrawTextA(NULL, buffer, -1, &r1, flags | DT_NOCLIP, m_textoutline_color);
			r1 = { r.left + 1, r.top - 0, r.right, r.bottom };
			m_pfont->DrawTextA(NULL, buffer, -1, &r1, flags | DT_NOCLIP, m_textoutline_color);
			r1 = { r.left + 1, r.top + 1, r.right, r.bottom };
			m_pfont->DrawTextA(NULL, buffer, -1, &r1, flags | DT_NOCLIP, m_textoutline_color);
			r1 = { r.left + 0, r.top + 1, r.right, r.bottom };
			m_pfont->DrawTextA(NULL, buffer, -1, &r1, flags | DT_NOCLIP, m_textoutline_color);
			r1 = { r.left - 1, r.top + 1, r.right, r.bottom };
			m_pfont->DrawTextA(NULL, buffer, -1, &r1, flags | DT_NOCLIP, m_textoutline_color);
			r1 = { r.left - 1, r.top - 0, r.right, r.bottom };
			m_pfont->DrawTextA(NULL, buffer, -1, &r1, flags | DT_NOCLIP, m_textoutline_color);
		}

		//main text
		auto clr = color2dx(color, alpha);
		m_pfont->DrawTextA(NULL, buffer, -1, &r, flags | DT_NOCLIP, clr);
	}

	std::string format_a(const char* string, ...) {

		static char buffer[256] = { 0 };
		va_list arg_list;
		va_start(arg_list, string);
		vsnprintf(buffer, sizeof(buffer), string, arg_list);
		va_end(arg_list);

		return buffer;
	}

	std::wstring format_w(const wchar_t* string, ...) {

		wchar_t buffer[256] = { 0 };
		va_list args;
		va_start(args, string);
		vswprintf(buffer, sizeof(buffer) / 2, string, args);
		va_end(args);

		return buffer;
	}

	void group_box(float x, float y, float w, float h, float tx, float tx1, const color_t& color, float alpha = 255) {

		auto dxcolor = color2dx(color, alpha);

		w--;
		h--;

		std::vector<v::transformed> vertices = {
			{tx, y,0,1,dxcolor},
			{x,y,0.f,1.f,dxcolor},
			{x,y + h,0.f,1.f,dxcolor},
			{x + w,y + h,0.f,1.f,dxcolor},
			{x + w,y,0.f,1.f,dxcolor},
			{tx1,y,0.f,1.f,dxcolor}
		};

		m_pdevice->SetFVF(fvf::transformed);
		m_pdevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 5, &vertices[0], sizeof v::transformed);
	}

	void rect_textured(float x, float y, float x1, float y1, const color_t& color, float alpha)
	{
		auto dxcolor = color2dx(color, alpha);

		std::vector<v::transformed_uv> vertices = {
		{x,y,1.f,1.f,dxcolor,0,0},
		{x1,y,1.f,1.f,dxcolor,1,0},
		{x1,y1,1.f,1.f,dxcolor,1,1},
		{x,y1,1.f,1.f,dxcolor,0,1},
		};

		m_pdevice->SetFVF(fvf::transformed_uv);
		m_pdevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &vertices[0], sizeof v::transformed_uv);
	}
	
	void load_cursor(void* data, unsigned int size)
	{
		D3DXIMAGE_INFO info = { 0 };
		D3DXGetImageInfoFromFileInMemory(data, size, &info);
		auto a = m_pdevice->CreateOffscreenPlainSurfaceEx(32, 32, info.Format, D3DPOOL_DEFAULT, &m_pcursorsurface, NULL, 0);
	
		a = D3DXLoadSurfaceFromFileInMemory(m_pcursorsurface, NULL, NULL, data, size, NULL, D3DX_DEFAULT, 0, NULL);
	}
}