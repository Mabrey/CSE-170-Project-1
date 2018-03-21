
# include "my_viewer.h"
# include "sn_mytorus.h"

# include <sigogl/gl_tools.h>
# include <sigogl/ui_button.h>

int rd = 100;
int g = 100;
int b = 100;
double R = 0.1;
double r = 0.04;
double res = 6;


MyViewer::MyViewer ( int x, int y, int w, int h, const char* l ) : WsViewer(x,y,w,h,l)
{
	add_ui ();
	add_mynode (4);
}

void MyViewer::add_ui ()
{
	UiPanel *p;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel ( "", UiPanel::HorizLeft );
	p->add ( new UiButton ( "Add", EvAdd ) );
	p->add ( new UiButton ( "Info", EvInfo ) );
	p->add ( new UiButton ( "Exit", EvExit ) );
}

void MyViewer::add_mynode ( int n )
{
	SnMyTorus* c;

	float r=0.15f; // position range
	while ( n-->0 )
	{	c = new SnMyTorus;
		c->init.set ( gs_random(-r,r), gs_random(-r,r), gs_random(-r,r) );
		c->width = 0.5;
		c->height = 1.0;
		c->color ( GsColor(100, 100, 100 ));
		// Example how to print/debug your generated data:
		// gsout<<n<<": "<<c->color()<<gsnl;
		rootg()->add(c);
	}
}

int MyViewer::handle_keyboard ( const GsEvent &e )
{
	SnMyTorus* c = new SnMyTorus;
	int ret = WsViewer::handle_keyboard ( e ); // 1st let system check events
	if ( ret ) return ret;
	c->res = res;
	c->r = r;
	c->R = R;

	switch ( e.key )
	{	case GsEvent::KeyEsc : gs_exit(); return 1;
		case GsEvent::KeyLeft: gsout<<"Left\n"; return 1;
		case GsEvent::KeyCodes(113)://q
			gsout << "res: " << res << "\n";
			res += 1;
			c->res = res;
			//c->scaleRes(1, res);
			c->color(GsColor(rd, g, b));
			rootg()->remove_all();
			rootg()->add(c);
			WsViewer::render();
			return 1;

		case GsEvent::KeyCodes(97)://a
			
			if (res > 4)
			{
				res += -1;
				c->res = res;
				//c->scaleRes(-1, res);
				c->color(GsColor(rd, g, b));
				rootg()->remove_all();
				rootg()->add(c);
				WsViewer::render();
			}
			gsout << "res: " << res << "\n";
			return 1;

		case GsEvent::KeyCodes(119)://w
			r += 0.01;
			c->r = r;
			//c->scale_r(0.01, r);
			c->color(GsColor(rd, g, b));
			rootg()->remove_all();
			rootg()->add(c);
			WsViewer::render();
			gsout << "r: " << r << "\n";
			return 1;

		case GsEvent::KeyCodes(115)://s
			r += -0.01;
			c->r = r;
			//c->scale_r(-0.01, r);
			c->color(GsColor(rd, g, b));
			rootg()->remove_all();
			rootg()->add(c);
			WsViewer::render();
			gsout << "r: " << r << "\n";
			return 1;

		case GsEvent::KeyCodes(101)://e
			R += 0.05;
			c->R = R;
			//c->scaleR(0.05, R);
			c->color(GsColor(rd, g, b));
			rootg()->remove_all();
			rootg()->add(c);
			WsViewer::render();
			return 1;

		case GsEvent::KeyCodes(100)://d
			R += -0.05;
			c->R = R;
			//c->scaleR(-0.05, R);
			c->color(GsColor(rd, g, b));
			rootg()->remove_all();
			rootg()->add(c);
			WsViewer::render();
			return 1;

		case GsEvent::KeyCodes(114):
			gsout << "Red\n";
			rd = 255;
			g = 0;
			b = 0;
			c->color(GsColor(rd, g, b));
			rootg()->remove();
			rootg()->add(c);
			WsViewer::render();
			return 1;
		
		case GsEvent::KeyCodes(103):
			gsout << "Green\n";
			rd = 0;
			g = 255;
			b = 0;
			c->color(GsColor(rd, g, b));
			rootg()->remove();
			rootg()->add(c);
			WsViewer::render();
			return 1;
		
		case GsEvent::KeyCodes(98):
			gsout << "Blue\n";
			rd = 0;
			g = 0;
			b = 255;
			c->color(GsColor(rd, g, b));
			rootg()->remove();
			rootg()->add(c);
			WsViewer::render();
			return 1;

		// etc
		default: gsout<<"Key pressed: "<<e.key<<gsnl;
	}

	return 0;
}

int MyViewer::uievent ( int e )
{
	switch ( e )
	{	case EvAdd: add_mynode(1); return 1;

		case EvInfo:
		{	if ( output().len()>0 ) { output(""); return 1; }
			output_pos(0,30);
			activate_ogl_context(); // we need an active context
			GsOutput o; o.init(output()); gl_print_info(&o); // print info to viewer
			return 1;
		}

		case EvExit: gs_exit();
	}
	return WsViewer::uievent(e);
}
