# AMF: Attention-based Multi-task Field-probing Networks for 3D Data

Semantic learning on 3D geometric data. 3D objects classification. Dataset: <a href="https://vision.princeton.edu/projects/2014/3DShapeNets/" target="_blank">ModelNet40</a>.

## Introduction
We propose an efficient way for semantic learning from 3D data, based on <a href="http://arxiv.org/abs/1605.06240" target="_blank">FPNN</a> (was accepted to NIPS 2016). FPNN utilizes probing filters to extract global features and greatly reduces the computational cost. However, it fails to pay attention to local features and weights are not shared among filters as well. By designing **AMF**, we aim to combine global features with local features and find weights to coordinate among probing filters.

## Usage
Check <a href="https://github.com/liuyuxiang512/AMF/tree/master/training_settings" target="_blank">training settings</a> for example usage of the field probing layers.

Refer to <a href="https://github.com/liuyuxiang512/AMF/blob/master/instructions.txt" target="_blank">instructions</a> for installing and running of the codes.
