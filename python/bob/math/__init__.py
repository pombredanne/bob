from ..core import __from_extension_import__
__from_extension_import__('._math', __name__, locals())

# add some default distance functions
import math
import numpy

__all__ = [k for k in dir() if not k.startswith('_')]
if 'k' in locals(): del k
