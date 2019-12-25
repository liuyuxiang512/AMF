# AMF: Attention-based Multi-task Field-probing Networks for 3D Data

Semantic learning on 3D geometric data. 3D objects classification. Dataset: ModelNet40.

## Introduction
We propose an efficient way for semantic learning from 3D data, based on <a href="http://arxiv.org/abs/1605.06240" target="_blank">FPNN</a> (was accepted to NIPS 2016). FPNN utilizes probing filters to extract global features and greatly reduces the computational cost. However, it fails to pay attention to local features and weights are not shared among filters as well. By designing **AMF**, we aim to combine global features with local features and find weights to coordinate among probing filters.

## Usage
Check <a href="https://github.com/liuyuxiang/AMF/training_settings" target="_blank">training settings</a> for example usage of the field probing layers.
