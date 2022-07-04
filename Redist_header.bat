@echo off
chcp 65001

rmdir /s /q Redist
mkdir Redist

mkdir Redist\FeatureCatalog
mkdir Redist\GeoMetryLibrary
mkdir Redist\GISLibrary
mkdir Redist\LatLonUtility
mkdir Redist\LibMFCUtil
mkdir Redist\LuaScriptingReference
mkdir Redist\PortrayalCatalogue
mkdir Redist\S100Engine
mkdir Redist\S100Geometry

xcopy /q FeatureCatalog\*.h Redist\FeatureCatalog
xcopy /q GeoMetryLibrary\*.h Redist\GeoMetryLibrary
xcopy /q GISLibrary\*.h Redist\GISLibrary
xcopy /q LatLonUtility\*.h Redist\LatLonUtility
xcopy /q LibMFCUtil\*.h Redist\LibMFCUtil
xcopy /q LuaScriptingReference\*.h Redist\LuaScriptingReference
xcopy /q PortrayalCatalogue\*.h Redist\PortrayalCatalogue
xcopy /q S100Engine\*.h Redist\S100Engine
xcopy /q S100Geometry\*.h Redist\S100Geometry

REM mkdir Redist\extlibs

REM xcopy /q /e extlibs Redist\extlibs