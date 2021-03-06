// Copyright (c) 2018 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "atom/browser/api/atom_api_web_contents.h"

#include "content/browser/frame_host/frame_tree.h"
#include "content/browser/frame_host/frame_tree_node.h"
#include "content/browser/web_contents/web_contents_impl.h"
#include "content/public/browser/guest_mode.h"

#if BUILDFLAG(ENABLE_OSR)
#include "atom/browser/osr/osr_render_widget_host_view.h"
#include "atom/browser/osr/osr_web_contents_view.h"
#endif

// Including both web_contents_impl.h and node.h would introduce a error, we
// have to isolate the usage of WebContentsImpl into a clean file to fix it:
// error C2371: 'ssize_t': redefinition; different basic types

namespace atom {

namespace api {

void WebContents::DetachFromOuterFrame() {
  // See detach_webview_frame.patch on how to detach.
  DCHECK(content::GuestMode::IsCrossProcessFrameGuest(web_contents()));
  int frame_tree_node_id =
      static_cast<content::WebContentsImpl*>(web_contents())
          ->GetOuterDelegateFrameTreeNodeId();
  if (frame_tree_node_id != content::FrameTreeNode::kFrameTreeNodeInvalidId) {
    auto* node = content::FrameTreeNode::GloballyFindByID(frame_tree_node_id);
    DCHECK(node->parent());
    node->frame_tree()->RemoveFrame(node);
  }
}

#if BUILDFLAG(ENABLE_OSR)
OffScreenWebContentsView* WebContents::GetOffScreenWebContentsView() const {
  if (IsOffScreen()) {
    const auto* impl =
        static_cast<const content::WebContentsImpl*>(web_contents());
    return static_cast<OffScreenWebContentsView*>(impl->GetView());
  } else {
    return nullptr;
  }
}

OffScreenRenderWidgetHostView* WebContents::GetOffScreenRenderWidgetHostView()
    const {
  if (IsOffScreen()) {
    return static_cast<OffScreenRenderWidgetHostView*>(
        web_contents()->GetRenderWidgetHostView());
  } else {
    return nullptr;
  }
}
#endif

}  // namespace api

}  // namespace atom
