#include <Siv3D.hpp>
#include <stack>

const int WindowWidth = 1280;
const int WindowHeight = 960;

const int StageWidth = 16;
const int BlockSize = 32;

const int NoneNumber = 0;
const int OpenNumber = 1;
const int FlagNumber = 2;
const int BombNumber = 10;

const Point drawPos{ WindowWidth / 2 - BlockSize*StageWidth / 2, WindowHeight };

const Point direction[] = { Point(-1,-1),Point(0,-1),Point(1,-1),Point(-1,0),Point(1,0),Point(-1,1),Point(0,1),Point(1,1) };

class Control {
public:

	Control() {
		numberFont = Font(BlockSize / 2);

		stage.resize(10);
		for (auto& s : stage) s.resize(StageWidth, 0);
		openStage = stage;

		for (size_t y = 0; y < stage.size(); y++)
		{
			const auto& column = getColumn();
			for (size_t x = 0; x < StageWidth; x++)
				stage[y][x] = column[x];
		}

		stage[0][StageWidth / 2 - 1] = NoneNumber;
		stage[0][StageWidth / 2] = NoneNumber;
		stage[1][StageWidth / 2 - 1] = NoneNumber;
		stage[1][StageWidth / 2] = NoneNumber;

		openStage[0][StageWidth / 2 - 1] = 1;
		openStage[0][StageWidth / 2] = 1;
		openStage[1][StageWidth / 2 - 1] = 1;
		openStage[1][StageWidth / 2] = 1;

	}

	const bool update() {

		timer++;
		if (timer % 180 == 0)
		{
			const auto column = getColumn();
			stage.push_back(column);
			openStage.push_back(Array<int>(StageWidth));
		}

		const auto point2point = [&](const Point& pos) {
			const Point point = Point(pos.x - drawPos.x, drawPos.y - pos.y);
			const Point p = point / BlockSize;
			return p;
		};
		const auto in = [&](const Point& p) {
			return (0 <= p.x && p.x < StageWidth && 0 <= p.y && p.y < (int)stage.size());
		};

		if (Input::MouseL.released)
		{
			const Point mousePos = Mouse::Pos();
			const Point pos = point2point(mousePos);

			if (in(pos))
			{
				stageOpen(pos);
			}
		}
		else if (Input::MouseR.clicked)
		{
			const Point mousePos = Mouse::Pos();
			const Point pos = point2point(mousePos);

			if (in(pos))
			{
				if (openStage[pos.y][pos.x] == NoneNumber)
					openStage[pos.y][pos.x] = FlagNumber;
				else if (openStage[pos.y][pos.x] == FlagNumber)
					openStage[pos.y][pos.x] = NoneNumber;
			}
		}


		return true;
	}

	void draw() const {

		drawStage();

	}

private:

	Array<Array<int>> stage;
	Array<Array<int>> openStage;

	Font numberFont;

	int timer = 0;

	Array<int> getColumn() const {

		Array<int> column;
		for (int i = 0; i < StageWidth; i++)
			column.push_back(Random() <= 30.0 / 256 ? BombNumber : 0);

		return column;
	}

	const bool inside(const Point& p) const {
		return (0 <= p.x && p.x < StageWidth && 0 <= p.y && p.y < (int)stage.size());
	}

	void drawStage() const {

		for (size_t y = 0; y < stage.size(); y++)
		{
			for (size_t x = 0; x < stage[y].size(); x++)
			{
				int bombCount = 0;

				for (const auto& dire : direction)
				{
					const Point point = Point(x, y) + dire;
					if (inside(point))
					{
						if (stage[point.y][point.x] == BombNumber)
							bombCount++;
					}
				}

				Rect rect(drawPos.x + BlockSize*x, drawPos.y - BlockSize*(y + 1), BlockSize, BlockSize);

				if (openStage[y][x] == OpenNumber)
				{
					rect.draw(Palette::Lightblue);
					rect.top.draw(3, Palette::Aliceblue);
					rect.left.draw(1, Palette::Aliceblue);
					rect.bottom.draw(1, Palette::Darkblue);
					rect.right.draw(3, Palette::Darkblue);

					switch (bombCount)
					{
					case NoneNumber: break;
					case 1:
						numberFont(bombCount).drawCenter(rect.center, Palette::Black);
						break;
					case 2:
						numberFont(bombCount).drawCenter(rect.center, Palette::Black);
						break;
					case 3:
						numberFont(bombCount).drawCenter(rect.center, Palette::Black);
						break;
					case 4:
						numberFont(bombCount).drawCenter(rect.center, Palette::Black);
						break;
					case 5:
						numberFont(bombCount).drawCenter(rect.center, Palette::Black);
						break;
					case 6:
						numberFont(bombCount).drawCenter(rect.center, Palette::Black);
						break;
					case 7:
						numberFont(bombCount).drawCenter(rect.center, Palette::Black);
						break;
					case 8:
						numberFont(bombCount).drawCenter(rect.center, Palette::Black);
						break;
					default:
						numberFont(L"E").drawCenter(rect.center, Palette::Black);
						break;
					}

				}
				else if (openStage[y][x] == FlagNumber)
				{
					rect.draw(Palette::Deepskyblue);
					rect.top.draw(3, Palette::Lightblue);
					rect.left.draw(1, Palette::Lightblue);
					rect.bottom.draw(1, Palette::Mediumblue);
					rect.right.draw(3, Palette::Mediumblue);
					numberFont(L"F").drawCenter(rect.center, Palette::Black);
				}
				else
				{
					rect.draw(Palette::Deepskyblue);
					rect.top.draw(3, Palette::Lightblue);
					rect.left.draw(1, Palette::Lightblue);
					rect.bottom.draw(1, Palette::Mediumblue);
					rect.right.draw(3, Palette::Mediumblue);
				}

			}
		}

	}

	void stageOpen(const Point& pos) {

		if (openStage[pos.y][pos.x] == NoneNumber)
		{
			std::stack<Point> sta;
			sta.push(pos);

			while (!sta.empty())
			{
				const Point point = sta.top();
				sta.pop();
				openStage[point.y][point.x] = OpenNumber;

				bool bomb = false;
				for (const auto& d : direction)
				{
					const Point dp = Point(point.x, point.y) + d;
					if (inside(dp))
					{
						if (stage[dp.y][dp.x] == BombNumber)
						{
							bomb = true;
							break;
						}
					}
				}

				if (!bomb)
				{
					for (const auto& dire : direction)
					{
						const Point p = point + dire;
						if (inside(p))
						{
							if (openStage[p.y][p.x] == NoneNumber)
							{
								sta.push(p);
							}
						}
					}
				}
			}
		}
	}

};

void Main() {

	Window::Resize(WindowWidth, WindowHeight);

	Control control;

	bool setup = false;
	int anime = 0;

	while (System::Update())
	{
		if (!setup)
		{
			if (Input::MouseL.clicked)
			{
				setup = true;
				anime = 0;
			}
		}
		else
		{
			if (anime < 30)
			{
				if (anime < 15)
				{
					Rect(0, 0, WindowWidth, WindowHeight).draw(Color(0, 0, 0, 255 * anime / 15));
				}
				else
				{
					Rect(0, 0, WindowWidth, WindowHeight).draw(Color(0, 0, 0, 255 * (30 - anime) / 15));
				}

				anime++;
			}
			else
			{
				if (control.update())
				{
					control.draw();
				}
				else
				{
					control = Control();
					setup = false;
					anime = 0;
				}
			}
		}
	}
}
