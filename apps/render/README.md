# opens100-render

Draws S-100 datasets as a PNG file, with no window. One JSON file (a *job*) says everything:
which catalogues and datasets, where the view looks, how the chart is displayed, where the
picture goes. The same program and the same job give the same picture on Windows and Linux.

```
opens100-render <job.json>     draw what the job says
opens100-render --example      print a job to start from
opens100-render --help         the reference below, shorter
```

| Exit code | Meaning |
|---|---|
| 0 | The picture was written. |
| 1 | A file the job names could not be read, opened or written. |
| 2 | The job is wrong (not JSON, an unknown name, a value out of range). The message says which. |

Messages go to standard error and start with `opens100-render:`. On success four lines go to
standard output (picture, scale, centre, timing).

## A job

```json
{
  "catalogues": [
    {
      "featureCatalogue": "ProgramData/FC/S-101_FC.xml",
      "portrayalCatalogue": "ProgramData/PC/S101_Portrayal/portrayal_catalogue.xml",
      "context": { "SafetyDepth": "20", "SimplifiedSymbols": "false" }
    }
  ],
  "datasets": [ "data/101KR00659C71.000" ],
  "background": "ProgramData/World/World.shp",
  "fonts": [ "fonts/NotoSansKR-Regular.ttf" ],
  "view": {
    "size": [ 1600, 1000 ],
    "center": { "latitude": 35.10, "longitude": 129.04 },
    "scale": 25000
  },
  "display": { "palette": "Day", "mode": "all", "safetyContour": 30 },
  "output": { "picture": "chart.png", "report": "chart.json" }
}
```

Rules that hold for the whole file:

- A path is taken from the directory of the job file unless it is absolute. `/` works on every
  platform.
- A name the reference does not list is an error, and the message lists the names that are
  allowed there. A misspelt name is never ignored.
- `//` and `/* */` comments are allowed.
- Only `catalogues`, `datasets` and `output.picture` are required.

## Reference

### `catalogues` (required)

A list, one entry a product (S-101, S-102, S-124 ...). A dataset is drawn with the catalogues
of its product, so every product among the datasets needs its entry.

| Name | Type | |
|---|---|---|
| `featureCatalogue` | path | The feature catalogue XML. Required. |
| `portrayalCatalogue` | path | The `portrayal_catalogue.xml` of the portrayal catalogue; its rules, symbols and colour profiles are found next to it. Required. |
| `context` | object | Context parameters of that portrayal catalogue, `name: value`. A value may be a text, a number or `true`/`false`. A name the catalogue does not have is an error, and the message lists the ones it has. |

The context parameters of the S-101 portrayal catalogue in this repository: `SafetyDepth`,
`SafetyContour`, `FourShades`, `ShallowContour`, `DeepContour`, `ShallowWaterDangers`,
`PlainBoundaries`, `SimplifiedSymbols`, `FullLightLines`, `RadarOverlay`,
`IgnoreScaleMinimum`, `NationalLanguage`.

### `datasets` (required)

A list of dataset paths: S-101 cells (`.000`), GML datasets (`.gml`), HDF5 datasets (`.h5`),
an exchange catalogue (`CATALOG.XML`). They are drawn in this order, the last on top where the
drawing priorities are equal.

### `background`

Path of a shapefile (`.shp`, with its `.shx` next to it) drawn under the datasets.
`ProgramData/World/World.shp` of this repository is a map of the world.

### `fonts`

A list of font files (`.ttf`, `.otf`), the display font first. **Give it on Linux and
macOS**: with no font file the font is asked of the system by name, which only Windows
answers.

### `view`

| Name | Type | Default | |
|---|---|---|---|
| `size` | `[width, height]` | `[1600, 1000]` | Pixels, 16 to 16384 each way. |
| `projection` | text | `mercator` | `mercator`, `polarNorth` or `polarSouth` (polar stereographic). |
| `center` | `{ latitude, longitude }` | | Degrees, WGS 84, north and east positive. The middle of the picture. |
| `scale` | whole number | | The denominator: `25000` is 1:25 000. |
| `bbox` | `{ west, south, east, north }` | | Degrees. The view holds this range; it is widened one way to the shape of the picture. Not together with `center` or `scale`. |

What the view shows:

| Given | The view |
|---|---|
| nothing | all the datasets |
| `center` | that centre, at the scale that shows all the datasets |
| `scale` | that scale, about the middle of the datasets |
| `center` and `scale` | that centre at that scale |
| `bbox` | that range |

A pixel is taken to be 1/38 cm (about 96 dots an inch): at 1:25 000 a pixel is 6.58 m on the
ground and a picture 1600 pixels wide shows 10.5 km. The scale is true at the centre of the
view (Mercator enlarges the ground away from the equator).

The view is north up. A rotated view is not offered: see "Limits".

### `display`

| Name | Type | Default | |
|---|---|---|---|
| `palette` | text | `Day` | `Day`, `Dusk` or `Night`. |
| `mode` | text | `all` | `base`, `standard` or `all`: the display category shown. |
| `text` | boolean | `true` | Draw texts. |
| `fullSectors` | boolean | `false` | Light sectors at their full length. |
| `applyScaleMin` | boolean | `true` | Hide a feature when the view is smaller than its scale minimum. |
| `safetyContour` | number | `30` | Metres. |
| `shallowContour` | number | `10` | Metres. |
| `deepContour` | number | `30` | Metres. |
| `symbolScale` | number | `5` | Size of symbols. |
| `fontSize` | whole number | `15` | Size of texts. |
| `fontName` | text | `Malgun Gothic` | Font asked of the system when `fonts` is empty (Windows). |

The three contours are used by the coverage products (S-102) and are also given to each
catalogue that has the context parameter `SafetyContour`, `ShallowContour` or `DeepContour`. A
`context` entry of a catalogue wins over them for that catalogue.

### `output` (required)

| Name | Type | Default | |
|---|---|---|---|
| `picture` | path | | The PNG file. Required. Its directory is made when it is missing. |
| `background` | `"#RRGGBB"` | white; black at Dusk and Night | The colour where nothing is drawn. |
| `report` | path | | A JSON file telling what was drawn. |

## The report

Written when `output.report` is given, also when the run failed (then `drawn` is `false` and
`error` holds the message).

```json
{
  "drawn": true,
  "picture": "/work/chart.png",
  "size": [ 1600, 1000 ],
  "palette": "Day",
  "scale": 25000,
  "center": { "latitude": 35.10, "longitude": 129.04 },
  "bbox": { "west": 128.98, "south": 35.07, "east": 129.09, "north": 35.12 },
  "catalogues": [ { "featureCatalogue": "...", "portrayalCatalogue": "...", "context": { "SafetyDepth": "20" } } ],
  "datasets": [ { "path": "/work/data/101KR00659C71.000", "opened": true } ],
  "milliseconds": { "open": 850, "draw": 640 }
}
```

`scale`, `center` and `bbox` are those of the picture as drawn, so a first run with no `view`
tells where the datasets are: take its `center` and choose a `scale` for the next job.
`bbox` is the lower left and the upper right corner of the picture. `context` lists the
parameters that were set, including the contours taken from `display`.

## Recipes

Find out where a dataset is, then look closer:

```json
{ "catalogues": [ ... ], "datasets": [ "cell.000" ],
  "output": { "picture": "all.png", "report": "all.json" } }
```

```json
{ "catalogues": [ ... ], "datasets": [ "cell.000" ],
  "view": { "center": { "latitude": 35.11, "longitude": 129.04 }, "scale": 8000 },
  "output": { "picture": "close.png" } }
```

The same place at night, base display, no texts:

```json
"display": { "palette": "Night", "mode": "base", "text": false }
```

Two products in one picture (each needs its catalogues):

```json
"catalogues": [
  { "featureCatalogue": "FC/S-101_FC.xml", "portrayalCatalogue": "PC/S101_Portrayal/portrayal_catalogue.xml" },
  { "featureCatalogue": "FC/S-124_FC.xml", "portrayalCatalogue": "PC/S124_Portrayal/portrayal_catalogue.xml" }
],
"datasets": [ "cell.000", "warning.gml" ]
```

## Getting the program

With the vcpkg port of this repository (the feature `apps`; it brings Skia):

```
vcpkg install "opens100[core,apps]" --overlay-ports=<OpenS100>/ports
<vcpkg installed>/<triplet>/tools/opens100/opens100-render --help
```

In a build of this repository with CMake: `-DOPENS100_BUILD_RENDER_SKIA=ON
-DOPENS100_BUILD_APPS=ON`, with the manifest features `render-skia;apps`.

## Limits

- One process draws one job. The display settings are those of the process.
- No rotated view. The library turns point symbols and area fills differently when the view is
  rotated off screen; until that is understood the program does not offer it.
- `background` is one shapefile.
- Where the datasets do not cover the picture and there is no background, the colour is
  `output.background`.
- macOS has not been tried.
