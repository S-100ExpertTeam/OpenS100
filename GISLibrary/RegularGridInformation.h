#pragma once

namespace S100HDF5
{

	class RegularGridInformation
	{
	public:
		RegularGridInformation();
		virtual ~RegularGridInformation();

		void SetNumPointLongitudinal(int num);
		int GetNumPointLongitudinal();

		void SetNumPointLatitudinal(int num);
		int GetNumPointLatitudinal();

		void SetGridSpacingLongitudinal(float spacing);
		float GetGridSpacingLongitudinal();

		void SetGridSpacingLatitudinal(float spacing);
		float GetGridSpacingLatitudinal();

		void SetGridOriginLongitude(float origin);
		float GetGridOgirinLongitude();

		void SetGridOriginLatitude(float origin);
		float GetGridOriginLatitude();

	private:
		int _NumPointLongitudinal = 0;
		int _NumPointLatitudinal = 0;
		float _GridSpacingLongitudinal = 0;
		float _GridSpacingLatitudinal = 0;
		float _GridOriginLongitude = 0;
		float _GridOriginLatitude = 0;
	};

}