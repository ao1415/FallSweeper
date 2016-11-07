#include <Siv3D.hpp>

const int StageWidth = 16;
const int BlockSize = 24;

const int BombNumber = 10;

class Control {
public:

	Control() {
		stage.resize(10);
		for (auto& s : stage) s.resize(StageWidth);

		for (size_t y = 0; y < stage.size(); y++)
		{
			const auto& column = getColumn();
			for (size_t x = 0; x < StageWidth; x++)
				stage[y][x] = column[x];
		}

	}

	void update() {

		if (Input::KeyEnter.clicked)
		{
			const auto column = getColumn();
			stage.push_back(column);
		}

	}

	void draw() const {
		for (size_t y = 0; y < stage.size(); y++)
		{
			for (size_t x = 0; x < stage[y].size(); x++)
			{
				Rect(x*BlockSize, y*BlockSize, BlockSize, BlockSize).draw(Palette::Gray);
				Rect(x*BlockSize, y*BlockSize, BlockSize, BlockSize).drawFrame(1, 0, Palette::White);
				PutText(stage[y][x]).from(x*BlockSize, y*BlockSize);
			}
		}
	}

private:

	Array<Array<int>> stage;

	Array<int> getColumn() const {

		Array<int> column;
		for (int i = 0; i < StageWidth; i++)
			column.push_back(Random() <= 0.15625 ? BombNumber : 0);

		return column;
	}

};

void Main() {

	Control control;

	while (System::Update())
	{
		control.update();
		control.draw();
	}
}
