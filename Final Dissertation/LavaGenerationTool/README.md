This folder contains the pipeline that runs the semi automatic tool by using the functions in the .py folders. The folders LavaCreation.py, ImageGeneration.py and segmentation.py is code I wrote whereas Lee.py sourced from Gerard Gallardo i Peres.

TestLavaImages is a folder with numpy arrays of radar images pre downloaded and corrected, using the code in DownloadingData, that can be used by the tool in the pipeline.

Model_weights contains that weights that have been collected from training the model in TrainingGANcode, this is what is used in the pipeline to avoid having to train the model again.