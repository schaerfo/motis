<script lang="ts">
	import Layer from '$lib/map/Layer.svelte';
	import GeoJSON from '$lib/map/GeoJSON.svelte';
	import type { Itinerary, Leg, Mode, Place, PlanResponse } from '$lib/api/openapi';
	import { getColor } from '$lib/modeStyle';
	import polyline from '@mapbox/polyline';
	import { colord } from 'colord';

	export const PRECISION = 6;

	function isIndividualTransport(m: Mode): boolean {
		return m == 'WALK' || m == 'BIKE' || m == 'CAR';
	}

	function getIndividualModeColor(m: Mode): string {
		switch (m) {
			case 'CAR':
				return '#bf75ff';
			default:
				return '#42a5f5';
		}
	}

	function itineraryToGeoJSON(i: Itinerary): GeoJSON.GeoJSON {
		return {
			type: 'FeatureCollection',
			features: i.legs.flatMap((l) => {
				if (l.steps) {
					const color = isIndividualTransport(l.mode)
						? getIndividualModeColor(l.mode)
						: `${getColor(l)[0]}`;
					const outlineColor = colord(color).darken(0.2).toHex();
					return l.steps.map((p) => {
						return {
							type: 'Feature',
							properties: {
								color,
								outlineColor,
								level: p.fromLevel,
								way: p.osmWay
							},
							geometry: {
								type: 'LineString',
								coordinates: polyline.decode(p.polyline.points, PRECISION).map(([x, y]) => [y, x])
							}
						};
					});
				} else {
					const color = `${getColor(l)[0]}`;
					const outlineColor = colord(color).darken(0.2).toHex();
					return {
						type: 'Feature',
						properties: {
							outlineColor,
							color
						},
						geometry: {
							type: 'LineString',
							coordinates: polyline.decode(l.legGeometry.points, PRECISION).map(([x, y]) => [y, x])
						}
					};
				}
			})
		};
	}

	function placeToFeature(place: Place, l: Leg): GeoJSON.Feature {
		return {
			type: 'Feature',
			properties: {
				name: place.name,
				color: getColor(l)[0]
			},
			geometry: {
				type: 'Point',
				coordinates: [place.lon, place.lat]
			}
		};
	}

	function intermediateStopsToGeoJSON(i: Itinerary): GeoJSON.GeoJSON {
		return {
			type: 'FeatureCollection',
			features:
				// Add Point features for intermediate stops
				i.legs
					.filter((l) => !isIndividualTransport(l.mode))
					.flatMap((l) =>
						[l.from, ...(l.intermediateStops ?? []), l.to].map((p) => placeToFeature(p, l))
					)
		};
	}

	const {
		routingResponses,
		selectedItineraryIdx,
		level,
		theme
	}: {
		routingResponses: Array<Promise<PlanResponse>>;
		selectedItineraryIdx: [number, number] | undefined;
		level: number;
		theme: 'dark' | 'light';
	} = $props();

	//const geojson = $derived(itinerariesToGeoJSON(itineraries));
	//const intermediateStopsGeoJSON = $derived(intermediateStopsToGeoJSON(itineraries));
</script>

{#each routingResponses as r, rI (rI)}
	{#await r then r}
		{#each r.itineraries as it, i (i)}
			<!-- Leave out the selected itinerary here, we show it highlighted -->
			{#if selectedItineraryIdx == undefined || (selectedItineraryIdx != undefined && selectedItineraryIdx[0] != rI || selectedItineraryIdx[1] != i)}
			<GeoJSON id="route-{rI}-{i}" data={itineraryToGeoJSON(it)}>
				<Layer
					id="path-outline-{rI}-{i}"
					type="line"
					layout={{
						'line-join': 'round',
						'line-cap': 'round'
					}}
					filter={['any', ['!has', 'level'], ['==', 'level', level]]}
					paint={{
						'line-color': theme == 'dark' ? '#222' : '#aaa',
						'line-width': 10,
						'line-opacity': 0.8
					}}
				/>
				<Layer
					id="path-{rI}-{i}"
					type="line"
					layout={{
						'line-join': 'round',
						'line-cap': 'round'
					}}
					filter={['any', ['!has', 'level'], ['==', 'level', level]]}
					paint={{
						'line-color': theme == 'dark' ? '#555' : '#bbb',
						'line-width': 7.5,
						'line-opacity': 0.8
					}}
				/>
			</GeoJSON>
			{/if}
		{/each}
	{/await}
{/each}
{#if selectedItineraryIdx != undefined}
{#await routingResponses[selectedItineraryIdx[0]] then r}
	{#key r}
<GeoJSON id="route" data={itineraryToGeoJSON(r.itineraries[selectedItineraryIdx[1]])}>
	<Layer
		id="path-outline"
		type="line"
		layout={{
			'line-join': 'round',
			'line-cap': 'round'
		}}
		filter={['any', ['!has', 'level'], ['==', 'level', level]]}
		paint={{
			'line-color': ['get', 'outlineColor'],
			'line-width': 10,
			'line-opacity': 0.8
		}}
	/>
	<Layer
		id="path"
		type="line"
		layout={{
			'line-join': 'round',
			'line-cap': 'round'
		}}
		filter={['any', ['!has', 'level'], ['==', 'level', level]]}
		paint={{
			'line-color': ['get', 'outlineColor'],
			'line-width': 7.5,
			'line-opacity': 0.8
		}}
	/>
</GeoJSON>
<GeoJSON id="intermediate-stops" data={intermediateStopsToGeoJSON(r.itineraries[selectedItineraryIdx[1]])}>
	<Layer
		id="intermediate-stops"
		type="circle"
		layout={{}}
		filter={true}
		paint={{
			'circle-radius': 6,
			'circle-color': ['get', 'color'],
			'circle-stroke-width': 2,
			'circle-stroke-color': theme == 'dark' ? '#fff' : '#000'
		}}
	/>
	<Layer
		id="intermediate-stops-name"
		type="symbol"
		layout={{
			'text-field': ['get', 'name'],
			'text-font': ['Noto Sans Display Regular'],
			'text-size': 12,
			'text-offset': [0, 1.5],
			'text-anchor': 'top'
		}}
		filter={true}
		paint={{
			'text-halo-width': 2,
			// Use a darker hue of the line color for dark theme and a lighter hue for light theme
			'text-halo-color': [
				'interpolate-hcl',
				['linear'],
				0.6,
				0,
				['get', 'color'],
				1,
				theme == 'dark' ? '#000' : '#fff'
			],
			'text-color': theme == 'dark' ? '#fff' : '#000'
		}}
	/>
</GeoJSON>
	{/key}
{/await}
{/if}