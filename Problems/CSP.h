
#ifndef __PR_CSP_H__
#define __PR_CSP_H__

#include "..\source\FitnessValues.h"
#include "..\source\ChromosomeOperations.h"
#include "..\source\Representation.h"

namespace Problems
{
	namespace CSP
	{

		class Size;

		class Point
		{

		private:

			int _x;

			int _y;

		public:

			Point(int x, int y) : _x(x),
				_y(y) { }

			Point(const Point& src) : _x(src._x),
				_y(src._y) { }

			Point() : _x(0),
				_y(0) { }

			inline double Distance(const Point& point) const { return std::sqrt( std::pow( _x - point._x, 2 ) + std::pow( _y - point._y, 2 ) ); }

			inline void SetX(int x) { _x = x; }

			inline int GetX() const { return _x; }

			inline void SetY(int y) { _y = y; }

			inline int GetY() const { return _y; }

			Point& operator +=(const Size& rhs);

			Point& operator -=(const Size& rhs);

			Point& operator =(const Point& rhs);

			friend Point operator +(const Point& lhs, const Size& rhs);

			friend Size operator -(const Point& lhs, const Point& rhs);

			friend Point operator -(const Point& lhs, const Size& rhs);

			friend bool operator ==(const Point& lhs, const Point& rhs);

			friend bool operator !=(const Point& lhs, const Point& rhs);

		};

		class Size
		{

		private:

			int _width;

			int _height;

		public:

			Size(int width, int height) : _width(width),
				_height(height) { }

			Size(const Size& src) : _width(src._width),
				_height(src._height) { }

			Size() : _width(0),
				_height(0) { }

			inline void Rotate() { std::swap( _width, _height ); }

			inline void SetWidth(int width) { _width = width; }

			inline int GetWidth() const { return _width; }

			inline void SetHeight(int height) { _height = height; }

			inline int GetHeight() const { return _height; }

			inline int GetArea() const { return _width * _height; }

			inline Size GetRotated() const { return Size( _height, _width ); }

			inline Point ToPoint() const { return Point( _width, _height ); }
			
			inline bool Fits(const Size& size) const { return _width >= size._width && _height >= size._height; }

			inline int FitOriginal(const Size& size) const { return _width >= size._width && _height >= size._height ? ( *this - size ).GetArea() : -1; }

			int FitFirst(Size& size) const;

			int FitBest(Size& size) const;

			inline operator Point() const { return Point( _width, _height ); }

			Size& operator +=(const Size& rhs);

			Size& operator -=(const Size& rhs);

			Size& operator =(const Size& rhs);

			friend Size operator +(const Size& lhs, const Size& rhs);

			friend Point operator +(const Point& lhs, const Size& rhs);

			friend Size operator -(const Size& lhs, const Size& rhs);

			friend Point operator -(const Point& lhs, const Size& rhs);

			friend bool operator ==(const Size& lhs, const Size& rhs);

			friend bool operator !=(const Size& lhs, const Size& rhs);

		};

		inline Point operator +(const Point& lhs, const Size& rhs) { return Point( lhs._x + rhs._width, lhs._y + rhs._height ); }

		inline Size operator -(const Point& lhs, const Point& rhs) { return Size( lhs._x - rhs._x, lhs._y - rhs._y ); }

		inline Point operator -(const Point& lhs, const Size& rhs) { return Point( lhs._x - rhs._width, lhs._y - rhs._height ); }

		inline bool operator ==(const Point& lhs, const Point& rhs) { return lhs._x == rhs._x && lhs._y == rhs._y; }

		inline bool operator !=(const Point& lhs, const Point& rhs) { return lhs._x != rhs._x || lhs._y != rhs._y; }

		inline Size operator +(const Size& lhs, const Size& rhs) { return Size( lhs._width + rhs._width, lhs._height + rhs._height ); }

		inline Size operator -(const Size& lhs, const Size& rhs) { return Size( lhs._width - rhs._width, lhs._height - rhs._height ); }

		inline bool operator ==(const Size& lhs, const Size& rhs) { return lhs._width == rhs._width && lhs._height == rhs._height; }

		inline bool operator !=(const Size& lhs, const Size& rhs) { return lhs._width != rhs._width || lhs._height != rhs._height; }

		class Rectangle
		{

		private:

			Point _position;

			Size _size;

			Point _limit;

		public:

			Rectangle() { }

			Rectangle(const Point& position, const Size& size) : _position(position),
				_size(size),
				_limit(_position + _size)  { }

			inline void GetPosition(const Point& position) { _limit = ( _position = position ) + _size; }

			inline const Point& GetPosition() const { return _position; }

			inline void SetSize(const Size& size) { _limit = _position + ( _size = size ); }

			inline const Size& GetSize() const { return _size; }

			inline const Point& GetLimit() const { return _limit; }

			inline bool IsOverlapping(const Rectangle& r)
				{ return _position.GetX() < r._limit.GetX() && _limit.GetX() > r._position.GetX() && _position.GetY() < r._limit.GetY() && _limit.GetY() > r._position.GetY(); }

		};

		class Item
		{

		protected:

			Size _size;

			std::string _label;

			int _index;

		public:

			Item(const Size& size, const std::string& label, int index) : _size(size),
				_label(label),
				_index(index) { }

			Item() { }

			inline const Size& GetSize() const { return _size; }

			inline void SetSize(const Size& size) { _size = size; }

			inline const std::string& GetLabel() const { return _label; }

			inline void SetLabel(const std::string& label) { _label = label; }

			inline int GetIndex() const { return _index; }

			inline void SetIndex(int index) { _index = index; }

		};

		class Placement
		{

		private:

			const Item* _item;

			Rectangle _area;

		public:

			Placement(const Item& item) : _item(&item) { }

			Placement(const Item& item, const Point& position, const Size& rotation) : _item(&item),
				_area(position, rotation) { }

			inline const Item& GetItem() const { return *_item; }

			void SetArea(const Point& position, const Size& rotation) { _area = Rectangle( position, rotation ); }

			inline const Rectangle& GetArea() const { return _area; }

		};

		class Slot
		{

		private:

			Rectangle _area;

		public:

			Slot(const Point& position, const Size& size) : _area(position, size) { }

			inline const Rectangle& GetArea() const { return _area; }

			void Place(const Placement& placement, std::vector<Slot>& slots);

			bool Merge(const Slot& slot);

		};

		class Sheet
		{

		private:

			Size _size;

			std::vector<Placement> _placements;

			std::vector<Slot> _slots;

		public:

			Sheet(const Size& size) : _size(size) { _slots.push_back( Slot( Point(), _size ) ); }

			Sheet(const Sheet& src) : _size(src._size),
				_placements(src._placements),
				_slots(src._slots) { }

			template<typename HEURISTIC>
			inline bool Place(const HEURISTIC& heuristic, const Item& item, bool rotation) { return Place( heuristic, item, item.GetSize(), rotation ); }

			template<typename HEURISTIC>
			bool Place(const HEURISTIC& heuristic, const Item& item, const Size& orientation, bool rotation)
			{
				Placement placement( item );
				return heuristic( placement, orientation, rotation, _slots ) ? Place( placement ), true : false;
			}

			void Place(const Placement& placement);

			void Remove(Common::Data::GaSingleDimensionArray<int>& removed);

			void Clear();

			inline const Size& GetSize() const { return _size; }

			inline const std::vector<Placement>& GetPlacements() const { return _placements; }

			inline const std::vector<Slot>& GetSlots() const { return _slots; }

			Sheet& operator =(const Sheet& rhs);

		};

		bool ClosesDistanceHeuristic(Placement& placement,
			Size orientation,
			bool rotation,
			const std::vector<Slot>& slots);
		bool LowestPositionHeuristic(Placement& placement,
			Size orientation,
			bool rotation,
			const std::vector<Slot>& slots);
		bool BestFitHeuristic(Placement& placement,
			Size orientation,
			bool rotation,
			const std::vector<Slot>& slots);
		
		void PlaceItems(Sheet& sheet,
			const Common::Data::GaSingleDimensionArray<Item>& items,
			const Common::Data::GaSingleDimensionArray<int>& order);

		class CspConfigBlock : public Chromosome::GaChromosomeConfigBlock
		{

		private:
			
			Common::Data::GaSingleDimensionArray<Item> _items;

			Size _sheetSize;

		public:

			CspConfigBlock(const Common::Data::GaSingleDimensionArray<Item>& items, const Size& sheetSize) : _sheetSize(sheetSize) { SetItems( items ); }

			CspConfigBlock(const CspConfigBlock& rhs) : GaChromosomeConfigBlock(rhs),
				_sheetSize(rhs._sheetSize) { SetItems( rhs._items ); }

			virtual GaChromosomeConfigBlock* GACALL Clone() const { return new CspConfigBlock( *this ); }

			inline const Common::Data::GaSingleDimensionArray<Item>& GACALL GetItems() const { return _items; }

			inline void GACALL SetItems(const Common::Data::GaSingleDimensionArray<Item>& items) { _items = items; }

			inline void GACALL SetSheetSize(const Size& size) { _sheetSize = size; }

			inline const Size& GACALL GetSheetSize() const { return _sheetSize; }

		};

		typedef Chromosome::Representation::GaSDAChromosome<int>::GaType CspChromosome;

		class CspInitializator : public Chromosome::GaInitializator
		{

		public:

			virtual Chromosome::GaChromosomePtr GACALL operator ()(bool empty,
				const Chromosome::GaInitializatorParams& parameters,
				Common::Memory::GaSmartPtr<Chromosome::GaChromosomeConfigBlock> configBlock) const;

			virtual Common::GaParameters* GACALL CreateParameters() const { return NULL; }

		};

		typedef Fitness::Representation::GaSVFitness<float> CspFitness;

		class CspFitnessOperation : public Chromosome::GaChromosomeFitnessOperation
		{

		public:

			virtual Fitness::GaFitness* GACALL CreateFitnessObject(Common::Memory::GaSmartPtr<const Fitness::GaFitnessParams> params) const { return new CspFitness( params ); }

			virtual void GACALL operator ()(const GaObjectType& object,
				Fitness::GaFitness& fitness,
				const Fitness::GaFitnessOperationParams& operationParams) const;

			virtual Common::GaParameters* GACALL CreateParameters() const  { return NULL; }

		};

		class CspCrossoverOperation : public Chromosome::GaCrossoverOperation
		{

		public:

			virtual void GACALL operator ()(Chromosome::GaCrossoverBuffer& crossoverBuffer,
				const Chromosome::GaCrossoverParams& parameters) const;

			virtual int GACALL GetParentCount(const Chromosome::GaCrossoverParams& parameters) const { return 2; }

			virtual int GACALL GetOffspringCount(const Chromosome::GaCrossoverParams& parameters) const { return 2; }

			virtual Common::GaParameters* GACALL CreateParameters() const { return new Chromosome::GaCrossoverParams(); }

		};

	}
}

#endif // __PR_CSP_H__
