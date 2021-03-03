#include <cores/scene.h>
#include <renderer/renderer.h>
using namespace DR;
observer_ptr<Transform> IMPL::TransTable::get_tf(const Matrix4 &mat) const {
  value_type trans =
      std::make_unique<const Transform>(mat); // cost a Matrix::Inverse
  if (tf_table_.find(*trans) != tf_table_.end()) {
    return tf_table_[*trans].get();
  } else {
    value_type trans_inv =
        std::make_unique<const Transform>(Transform(*trans, true));
    auto res_ptr = trans.get(); // get the value before the unique_ptr is moved
    tf_table_[*trans] = std::move(trans);
    tf_table_[*trans_inv] = std::move(trans_inv);
    return res_ptr;
  }
}
observer_ptr<Transform> IMPL::TransTable::get_tf(const Transform &trans) const {
  if (tf_table_.find(trans) != tf_table_.end()) {
    return tf_table_[trans].get();
  } else {
    // if trans is a mat4(1.0), there will be only one entry in the table
    value_type trans_inv =
        std::make_unique<const Transform>(Transform(trans, true));
    value_type trans_ptr = std::make_unique<const Transform>(trans);
    tf_table_[trans] = std::move(trans_ptr);
    tf_table_[*trans_inv] = std::move(trans_inv);
    return tf_table_[trans].get();
  }
}
IMPL::TransTable::const_tf_pair
IMPL::TransTable::get_tf_and_inv(const Matrix4 &mat) const {
  auto trans = Transform(mat);
  auto res = std::pair(get_tf(trans), get_tf(trans.inverse()));
  assert_msg((res.first->m_ * res.second->m_).is_identity(),
             "got" << (res.first->m_ * res.second->m_));
  assert_msg((res.first->mInv_ * res.second->mInv_).is_identity(),
             "got" << res.first->mInv_ * res.second->mInv_);
  return res;
}
IMPL::TransTable::const_tf_pair
IMPL::TransTable::get_tf_and_inv(const Transform &trans) const {
  auto res = std::pair{get_tf(trans), get_tf(trans.inverse())};
  assert_msg((res.first->m_ * res.second->m_).is_identity(),
             "got" << (res.first->m_ * res.second->m_));
  assert_msg((res.first->mInv_ * res.second->mInv_).is_identity(),
             "got" << res.first->mInv_ * res.second->mInv_);
  return res;
}
void Scene::render() const {
  if (!renderer_) {
    std::cerr << "renderer is not parsed correctly!" << std::endl;
    std::abort();
  }
  this->renderer_->render(*this);
}
