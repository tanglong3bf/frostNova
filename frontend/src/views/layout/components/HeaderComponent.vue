<script lang="ts" setup>
import { defineEmits, defineProps } from 'vue'
import { useRoute } from 'vue-router'
import { Expand, Fold, FullScreen } from '@element-plus/icons-vue'

const route = useRoute()

// 和侧边栏菜单相关
const props = defineProps({
  asideCollapse: {
    type: Boolean,
    required: true,
    value: false,
  }
})

const emits = defineEmits(['toggleAsideCollapse'])
const toggleAsideCollapse = () => {
  emits('toggleAsideCollapse')
}
</script>

<template>
  <div class="header-left">
    <el-icon
        class="collapse-btn"
        @click="toggleAsideCollapse"
    >
      <Fold v-if="!asideCollapse"/>
      <Expand v-else/>
    </el-icon>
    <el-breadcrumb separator="/">
      <el-breadcrumb-item :to="{ path: '/home' }">首页</el-breadcrumb-item>
      <template v-if="route.meta.title && route.name !== 'home'">
        <el-breadcrumb-item>{{ route.meta.title }}</el-breadcrumb-item>
      </template>
    </el-breadcrumb>
  </div>
  <div class="header-right">
    <el-tooltip content="全屏" placement="bottom">
      <el-icon class="header-icon"><FullScreen /></el-icon>
    </el-tooltip>

    <div class="user-info">
      <el-avatar :size="28" src="https://cube.elemecdn.com/0/88/03b0d39583f48206768a7534e55bcpng.png" />
      <span class="username">管理员</span>
    </div>
  </div>
</template>

<style lang="scss" scoped>
.header-left {
  display: flex;
  align-items: center;
  gap: 16px;
  .collapse-btn {
    font-size: 18px;
    cursor: pointer;
    color: #666;
    transition: all 0.3s;
    width: 36px;
    height: 36px;
    display: flex;
    align-items: center;
    justify-content: center;
    border-radius: 8px;
    background-color: transparent;
    &:hover {
      color: #1890ff;
      background-color: #f0f7ff;
    }
  }
  :deep(.el-breadcrumb__inner) {
    color: #666;
    font-weight: normal;
    font-size: 14px;
  }
}

.header-right {
  position: relative;
  display: flex;
  align-items: center;
  gap: 8px;
  .header-icon {
    font-size: 16px;
    cursor: pointer;
    color: #666;
    width: 32px;
    height: 32px;
    display: flex;
    align-items: center;
    justify-content: center;
    border-radius: 6px;
    transition: all 0.3s;
    background-color: transparent;
    &:hover {
      color: #1890ff;
      background-color: #f0f7ff;
    }
  }
  .user-info {
    display: flex;
    align-items: center;
    padding: 0 8px;
    height: 32px;
    border-radius: 6px;
    .username {
      margin: 0 4px;
      font-size: 14px;
      color: #333;
      font-weight: 500;
    }
    :deep(.el-avatar) {
      border: 2px solid #fff;
      box-shadow: 0 0 4px rgba(0,0,0,.1);
      transition: all 0.3s;
    }
  }
}
</style>
