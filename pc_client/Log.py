#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from loguru import logger

logger.add("log.log", rotation="10 MB")
