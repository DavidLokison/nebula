nebula_begin_module(graphicsfeature)
nebula_flatc(SYSTEM graphicsfeature/graphicsfeatureschema.fbs graphicsfeature/terrainschema.fbs graphicsfeature/vegetationschema.fbs)
fips_deps(render application dynui tbui nflatbuffer)
target_precompile_headers(graphicsfeature PRIVATE <application/stdneb.h>)
fips_ide_group(features)
fips_files(
	graphicsfeatureunit.h
	graphicsfeatureunit.cc
)
fips_dir(managers)
	fips_files(
		graphicsmanager.h
		graphicsmanager.cc
		cameramanager.h
		cameramanager.cc
	)
fips_dir(components)
	nebula_idl_compile(
		camera.json
		decal.json
		lighting.json
		model.json
	)
nebula_end_module()

