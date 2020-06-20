#ifndef GAL_OBJECT_IMPL_GAL_OBJECT_H_
#define GAL_OBJECT_IMPL_GAL_OBJECT_H_

namespace gal {
   
// TODO(colintan): Consider renaming to GALObject
class GALObjectBase {
public:
  bool IsValid() { return valid_; }

  void SetValid(bool valid) { valid_ = valid; }

private:
  bool valid_ = false;
};

} // namespace

#endif // GAL_OBJECT_IMPL_GAL_OBJECT_H_