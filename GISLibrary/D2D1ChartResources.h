#pragma once

namespace GISLibrary
{
	class D2D1Resources;
	class D2D1ChartResources
	{
	private:
		D2D1Resources* m_pResources = nullptr;
		ID2D1PathGeometry* m_pOwnShipGuardZone = nullptr;
		ID2D1PathGeometry* m_pRouteBufferZone = nullptr;

		D2D1_POINT_2F guardZone[4];

	public:
		D2D1ChartResources();
		virtual ~D2D1ChartResources();

		void SetChartResources(D2D1Resources* _resources);

		ID2D1PathGeometry* GetGuardZoneGeometry();
		ID2D1PathGeometry* MakeGuardZoneGeometry(D2D1_POINT_2F points[4]);
		ID2D1PathGeometry* GetRouteBufferZoneGeometry();
		void SetRouteBufferZoneGeometry(ID2D1PathGeometry* _geo);

		void SetGuardZone(D2D1_POINT_2F points[4]);
		D2D1_POINT_2F* GetGuardZone();
	};
}